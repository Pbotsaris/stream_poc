#include "audio_converter.hpp"
#include <cmath>
#include <iostream>

AudioSettings *AudioConverter::m_SETTINGS = AudioSettings::get_instance();

AudioConverter::AudioConverter() {

  m_codec = avcodec_find_encoder(m_SETTINGS->codec_id());
  is_valid_pointer(m_codec,
                   "Could not find audio codec. swapping to default: mp2");

  if (!m_valid) {
    m_valid = true;
    m_codec = avcodec_find_encoder(AV_CODEC_ID_MP2);
    is_valid_pointer(m_codec, "Could not get audio codec.");
  }

  if (m_valid) {
    m_codec_context = avcodec_alloc_context3(m_codec);
    is_valid_pointer(m_codec_context, "Could not allocate codec context.");
  }

  if (m_valid) {
    m_codec_context->bit_rate = m_SETTINGS->bitrate();
    m_codec_context->sample_fmt = m_SETTINGS->sample_format();
    validate_sample_format();
  }

  if (m_valid) {
    m_codec_context->sample_rate = m_SETTINGS->samplerate();
    //   validate_sample_rate();
  }

  if (m_valid) {
    set_channel_layout();

    int result = avcodec_open2(m_codec_context, m_codec, NULL);
    is_valid(result, "Could not open audio codec.");
  }

  if (m_valid) {
    m_packet = av_packet_alloc();
    is_valid_pointer(m_packet, "Could not allocate packet.");
  }

  if (m_valid) {
    setup_frame();
  }
}

/* */

std::size_t AudioConverter::frame_size_bytes() const { // in bytes

  return av_samples_get_buffer_size(
      NULL, m_codec_context->ch_layout.nb_channels, m_codec_context->frame_size,
      m_codec_context->sample_fmt, 0);
}

/* */

std::size_t AudioConverter::frame_size_samples() const {
  return m_codec_context->frame_size;
}

/* */

bool AudioConverter::valid() const { return m_valid; }

/* */

void AudioConverter::encode(AVData &t_avdata, uint8_t *t_audio_buffer,
                            std::size_t t_len) { // t_len in bytes

  std::size_t frame_size_in_bytes = frame_size_bytes();

  if (m_valid) {
    // here we get how many ffmpeg frames we need to convert a given SDL buffer
    // size.
    std::size_t nb_frames = std::ceil(t_len / frame_size_in_bytes) + 1;

    // offsets by the frame size at iteration.
    for (std::size_t i = 0; i < nb_frames; i++) {
      copy_to_frame(t_audio_buffer, frame_size_in_bytes,
                    frame_size_in_bytes * i);

      if (m_valid) {
        encode(t_avdata);
      }
    }

    std::cout << "calling flushing...\n";
    
    encode(t_avdata, true); // flush encoder.
  }
}

/* */

AudioConverter::~AudioConverter() { // free resources

  if (m_codec_context) {
    avcodec_free_context(&m_codec_context);
  }

  if (m_frame) {
    av_frame_free(&m_frame);
  }

  if (m_packet) {
    av_packet_free(&m_packet);
  }
}

/* Private */

void AudioConverter::encode(AVData &t_avdata, bool t_flush) {

  if (!m_valid) {
    return;
  }

  int result;

  // pass in a null AVFrame pointer to flush encoder.
  if (t_flush) {
    result = avcodec_send_frame(m_codec_context, m_flush_frame);
  } else {
    result =
        avcodec_send_frame(m_codec_context, m_frame); // send frame for encoding
  };

  is_valid(result, "Sending frame for encoding failed.");

  if (m_valid) {

    while (result >= 0) { // there may be many number of packets to read

      result = avcodec_receive_packet(m_codec_context, m_packet);

      // done converting..
      if (result == AVERROR(EAGAIN)) {
        std::cout << "Not enough data to decode. waiting on next frame.\n";
        m_awaiting = true;
        break;
      }

      else if (result == AVERROR_EOF) {
        std::cout << "EOF. Done.\n";
        m_awaiting = false;
        break;
      }

      else if (!is_valid(result, "Error encoding frame.")) {
        break;
      }

      t_avdata.load_audio(m_packet->data, m_packet->size);

      std::cout << "wrote...\n";
      m_awaiting = false;
      av_packet_unref(m_packet); // clean up packet
    }
  }
}

void AudioConverter::set_channel_layout() {

  AVChannelLayout layout;

  if (m_SETTINGS->is_mono()) {
    layout = AV_CHANNEL_LAYOUT_MONO;
  } else {

    layout = AV_CHANNEL_LAYOUT_STEREO;
  }

  av_channel_layout_copy(&m_codec_context->ch_layout, &layout);
}

void AudioConverter::setup_frame() {

  m_frame = av_frame_alloc();
  is_valid_pointer(m_frame, "Could not allocate frame.");

  if (m_valid) {

    m_frame->nb_samples = m_codec_context->frame_size;
    m_frame->format = m_codec_context->sample_fmt;
    int result = av_channel_layout_copy(&m_frame->ch_layout,
                                        &m_codec_context->ch_layout);

    is_valid(result, "Could not copy channel layout to frame.");
  }

  if (m_valid) {
    int result = av_frame_get_buffer(m_frame, 0);

    is_valid(result, "Could not allocate audio data buffer");
  }
}

/* */

void AudioConverter::copy_to_frame(uint8_t *t_audio_buffer,
                                   std::size_t t_frame_size_bytes,
                                   std::size_t t_offset) {

  int result = av_frame_make_writable(m_frame);

  is_valid(result, "Could not make frame writable.");
  uint8_t *buffer = m_frame->data[0];

  if (m_valid) {
    for (std::size_t i = 0; i < t_frame_size_bytes; i++) {
      buffer[i] = t_audio_buffer[t_offset + i]; // mono channel only.
    }
  }
}

void AudioConverter::write_zeros_to_frame(std::size_t t_frame_size_bytes) {

  int result = av_frame_make_writable(m_frame);

  is_valid(result, "Could not make frame writable.");
  uint8_t *buffer = m_frame->data[0];

  if (m_valid) {
    for (std::size_t i = 0; i < t_frame_size_bytes; i++) {
      buffer[i] = 0;
    }
  }
}

/* */

bool AudioConverter::validate_sample_rate() {

  if (m_codec->supported_samplerates) {
    for (int i = 0; m_codec->supported_samplerates[i]; i++) {
      if (m_codec_context->sample_rate == m_codec->supported_samplerates[i]) {
        return true;
      }
    }
  }

  std::cout << "Could not validate sample rate\n";
  m_valid = false;

  return m_valid;
}

/* */

bool AudioConverter::validate_sample_format() {

  const enum AVSampleFormat *sample_formats = m_codec->sample_fmts;
  const enum AVSampleFormat *first_format = sample_formats;

  while (*sample_formats != AV_SAMPLE_FMT_NONE) {
    if (*sample_formats == m_codec_context->sample_fmt) {
      return m_valid;
    }

    sample_formats++;
  }

  std::cout
      << "Format provided not available. Searching for the next option...\n";

  if (*first_format != AV_SAMPLE_FMT_NONE) {
    m_codec_context->sample_fmt = *first_format;
  } else {
    std::cout << "Error: Could not set a valid formating.";
    m_valid = false;
  }

  return m_valid;
}

/* */

template <typename T>
bool AudioConverter::is_valid_pointer(T t_pointer, const char *t_msg) {

  if (t_pointer == nullptr) {
    std::cout << t_msg << "\n";
    m_valid = false;
  }

  return m_valid;
}

/* */

bool AudioConverter::is_valid(int t_result, const char *t_msg) {
  if (t_result < 0) {

    // TODO: LOG Error
    m_valid = false;
    std::cout << t_msg << "\n";
  }

  return m_valid;
}
