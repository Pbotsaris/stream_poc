#include "AVConvert.hpp"

extern "C" { // must use to link
#include <libavutil/dict.h>
}

#include <iostream>
#include <type_traits>

AVConvert::AVConvert(const char *t_format, AudioSettings &&t_audio_settings,
                     VideoSettings &&t_video_settings)
    : m_audio_settings(t_audio_settings), m_video_settings(t_video_settings),
      m_audio_stream(new AVOutputStream), m_video_stream(new AVOutputStream) {

  avformat_alloc_output_context2(&m_format_context, nullptr, t_format, nullptr);

  if (!is_invalid_pointer(m_format_context)) {

    m_output_format = m_format_context->oformat;
    is_invalid_codec();
  }
}

AVConvert::~AVConvert() {

  if (m_format_context) {
    avformat_free_context(m_format_context);
  }

  delete m_audio_stream;
  delete m_video_stream;
}

/* */

void AVConvert::create_streams() {

  if (m_valid) {
    create_stream(m_audio_stream, m_output_format->audio_codec);
    create_stream(m_video_stream, m_output_format->video_codec);
    m_valid = m_video_stream->valid() && m_video_stream->valid();
  }
}

/* */

void AVConvert::open_audio() {

  if (m_valid) {
    set_options(m_audio_stream);
  }

  if (m_valid) {
    m_audio_stream->alloc_audio_frames(
        m_audio_settings
            .sample_format()); // allocs for both frame and temp frame
    m_audio_stream->alloc_resampler_context();
    m_audio_stream->init_resampler_context(m_audio_settings.sample_format());

    m_valid = m_audio_stream->valid();
  }

  if (m_valid) {
    stream_params_to_muxer(m_audio_stream);
  }
}

/* */

void AVConvert::open_video() {

  set_options(m_video_stream);

  if (m_valid) {
    m_video_stream->alloc_video_frame();
    m_valid = m_video_stream->valid();
  }

  if (m_valid) {
    stream_params_to_muxer(m_video_stream);
  }
}

/* */

void AVConvert::encode() { av_dump_format(m_format_context, 0, "stream", 1); }

/* Private */

void AVConvert::create_stream(AVOutputStream *t_stream, AVCodecID t_codec_id) {

  t_stream->find_codec(t_codec_id);

  if (t_stream->valid()) {
    t_stream->alloc_packets();
  }

  if (t_stream->valid()) {
    t_stream->stream = avformat_new_stream(m_format_context, NULL);
  }

  if (t_stream->valid()) {
    t_stream->set_stream_id(m_format_context->nb_streams - 1);
    t_stream->alloc_codec_context();
  }

  if (t_stream->valid() && t_stream->codec_type() == AVMEDIA_TYPE_AUDIO) {
    t_stream->set_audio_settings(m_audio_settings);
  }

  if (t_stream->valid() && t_stream->codec_type() == AVMEDIA_TYPE_VIDEO) {
     t_stream->set_video_settings(m_video_settings, t_codec_id);
  }
}

/* */

void AVConvert::set_options(AVOutputStream *t_stream) {

  AVDictionary *options = nullptr;

   av_dict_set(&m_options, "-b:a", "128K", 0);
   av_dict_set(&m_options, "-ar", "44100", 0);

  av_dict_copy(&options, m_options, 0);
  int result = avcodec_open2(t_stream->codec_context, t_stream->codec, &options);

  av_dict_free(&options);

  is_invalid(result, "Could not open audio or video.");
}

/* */

void AVConvert::stream_params_to_muxer(AVOutputStream *t_stream) {

  /* copy the stream parameters to the muxer */
  int result = avcodec_parameters_from_context(t_stream->stream->codecpar,
                                               t_stream->codec_context);
  is_invalid(result,
             "Could not copy params from streams to muxer."); // TODO: log error
}

/* */

template <typename T> bool AVConvert::is_invalid_pointer(T t_pointer) {

  if (t_pointer == nullptr) {
    // TODO: LOG Error
    m_valid = false;
  }

  return !m_valid;
}

/* */

bool AVConvert::is_invalid(int t_result, const char *t_msg) {
  if (t_result < 0) {

    // TODO: LOG Error
    m_valid = false;
    std::cout << t_msg << "\n";
  }

  return !m_valid;
}

/* */

bool AVConvert::is_invalid_codec() {

  if (m_output_format->audio_codec == AV_CODEC_ID_NONE ||
      m_output_format->video_codec == AV_CODEC_ID_NONE) {
    m_valid = false;
  }

  return !m_valid;
}
