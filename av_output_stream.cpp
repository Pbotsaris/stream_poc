#include "av_output_stream.hpp"
#include <iostream>
#include <libavutil/channel_layout.h>
#include <libavutil/frame.h>

bool AVOutputStream::valid() const { return m_valid; }

/* */

int AVOutputStream::codec_type() const { return codec->type; }
/* */

int AVOutputStream::number_audio_samples() const {
  return codec_context->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE
             ? AVOutputStream::m_DEFAULT_SAMPLE_NUMBER
             : codec_context->frame_size;
}

/* */

void AVOutputStream::set_audio_settings(AudioSettings &t_settings) {

  codec_context->sample_rate = t_settings.samplerate();
  codec_context->bit_rate = t_settings.bitrate();

  set_sample_format();

  if (codec->supported_framerates) {
    codec_context->sample_rate = codec->supported_samplerates[0];

    for (int i = 0; codec->supported_samplerates[i]; i++) {

      if (codec->supported_samplerates[i] == t_settings.samplerate()) {
        codec_context->sample_rate = t_settings.samplerate();
      }
    }
  } else {
    m_valid = false;
    // TODO: LOG
  }

  AVChannelLayout ch;

  ch = AV_CHANNEL_LAYOUT_STEREO;
  if (t_settings.channels() == 2) {
  } else {
    ch = AV_CHANNEL_LAYOUT_MONO;
  }

  av_channel_layout_copy(&codec_context->ch_layout, &ch);
  /*  1 secs / sample rate */
  stream->time_base = (AVRational){1, codec_context->sample_rate};
}

/* */

void AVOutputStream::set_video_settings(VideoSettings &t_settings,
                                        AVCodecID t_codec_id) {

  codec_context->codec_id = t_codec_id;
  codec_context->bit_rate = t_settings.bitrate();
  codec_context->width = t_settings.width();
  codec_context->height = t_settings.height();

  /* timebase: 1 sec / frame rate */
  stream->time_base = (AVRational){1, t_settings.framerate()};
  codec_context->gop_size =
      12; /* emit one intra frame every twelve frames at most */
  codec_context->time_base = stream->time_base;
  codec_context->pix_fmt = AV_PIX_FMT_YUV420P; /* default pix format */

  if (codec_context->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
    /* Needed to avoid using macroblocks in which some coeffs overflow.
     * This does not happen with normal video, it just happens here as
     * the motion of the chroma plane does not match the luma plane. */
    codec_context->mb_decision = 2;
  }
}

/* */

void AVOutputStream::find_codec(AVCodecID t_codec_id) {
  codec = avcodec_find_encoder(t_codec_id);
  is_valid_pointer(codec, "Did not find the codec.");
}

/* */

void AVOutputStream::alloc_packets() {
  packets = av_packet_alloc();
  is_valid_pointer(packets, "Could not allocate packets.");
}

void AVOutputStream::alloc_codec_context() {
  codec_context = avcodec_alloc_context3(codec);
  /* */
  is_valid_pointer(codec_context, "Could not allocate codec context.");
}

/* */

void AVOutputStream::set_stream_id(int t_id) { stream->id = t_id; }

/* */

void AVOutputStream::set_sample_format() { // bit depth
  codec_context->sample_fmt =
      codec->sample_fmts ? codec->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
}

/* */

void AVOutputStream::alloc_audio_frames(AVSampleFormat t_sample_format) {
  if (m_valid) {
    alloc_audio_frame(frame, codec_context->sample_fmt);
    alloc_audio_frame(frame_temp, t_sample_format);
  }
}

/* */

void AVOutputStream::alloc_video_frame() {

  frame = av_frame_alloc();

  if (is_valid_pointer(frame, "Could not allocate video frame.")) {

    frame->format = codec_context->pix_fmt;
    frame->width = codec_context->width;
    frame->height = codec_context->height;

    int result = av_frame_get_buffer(frame, 0);
    is_valid(result, "Could not get frame buffer.");
  }
}

/* */

void AVOutputStream::alloc_resampler_context() {
  resampler_context = swr_alloc();
  is_valid_pointer(resampler_context,
                   "Could not allocate resampler context."); // need log!
}

/* */

void AVOutputStream::init_conversion_context() {
  
  // if codec image pixel format is different than YUV420P we must convert
  if (codec_context->pix_fmt != AV_PIX_FMT_YUV420P) {
    conversion_context = sws_getContext(
        codec_context->width, codec_context->height, AV_PIX_FMT_YUV420P,
        codec_context->width, codec_context->height, codec_context->pix_fmt,
        AVOutputStream::m_CONVERSION_SCALE_FLAGS, NULL, NULL, NULL);

    is_valid_pointer(conversion_context,
                     "Could not get video conversion context.");
  }
}

/* */

void AVOutputStream::init_resampler_context(AVSampleFormat t_sample_format) {

  if (m_valid) {
    av_opt_set_chlayout(resampler_context, "in_chlayout",
                        &codec_context->ch_layout, 0);
    av_opt_set_int(resampler_context, "in_sample_rate",
                   codec_context->sample_rate, 0);
    av_opt_set_sample_fmt(resampler_context, "in_sample_fmt", t_sample_format,
                          0);
    av_opt_set_chlayout(resampler_context, "out_chlayout",
                        &codec_context->ch_layout, 0);
    av_opt_set_int(resampler_context, "out_sample_rate",
                   codec_context->sample_rate, 0);
    av_opt_set_sample_fmt(resampler_context, "out_sample_fmt",
                          codec_context->sample_fmt, 0);

    int result = swr_init(resampler_context);
    is_valid(result, "Could not initialize resampler.");
  }
}

/* Private */

void AVOutputStream::alloc_audio_frame(AVFrame *t_frame,
                                       AVSampleFormat t_sample_format) {

  int nb_samples = number_audio_samples();

  t_frame = av_frame_alloc();

  is_valid_pointer(t_frame, "Could not allocate audio frame.");

  if (m_valid) {
    t_frame->format = t_sample_format;
    av_channel_layout_copy(&t_frame->ch_layout, &codec_context->ch_layout);
    t_frame->sample_rate = codec_context->sample_rate;

    int result = av_frame_get_buffer(t_frame, 0);
    is_valid(result, "Could not get frame buffer.");
  }
}

/* */

template <typename T>
bool AVOutputStream::is_valid_pointer(T t_pointer, const char *t_msg) {

  if (t_pointer == nullptr) {
    m_valid = false;

    // TODO: LOG Error
    std::cout << t_msg << "\n";
  }

  return m_valid;
}

bool AVOutputStream::is_valid(int t_result, const char *t_msg) {

  if (t_result < 0) {
    m_valid = false;
    std::cout << t_msg << "\n";
  }

  return m_valid;
}

/* */

AVOutputStream::~AVOutputStream() {

  if (codec_context) {
    avcodec_free_context(&codec_context);
  }

  if (frame) {
    av_frame_free(&frame);
  }

  if (frame_temp) {
    av_frame_free(&frame_temp);
  }

  if (packets) {
    av_packet_free(&packets);
  }

  if (conversion_context) {
    sws_freeContext(conversion_context);
  }

  if (resampler_context) {
    swr_free(&resampler_context);
  }
}
