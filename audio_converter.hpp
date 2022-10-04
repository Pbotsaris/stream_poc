#ifndef AUDIO_CONVERTER_H
#define AUDIO_CONVERTER_H

extern "C" { // must use to link
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/frame.h>
#include <libavutil/samplefmt.h>
}

#include "av_settings.hpp"
#include "av_data.hpp"

class AudioConverter {

public:
  AudioConverter();
  std::size_t frame_size_bytes() const;
  std::size_t frame_size_samples() const;
  bool valid() const;

  void encode(AVData &t_avdata, uint8_t *t_audio_buffer, std::size_t t_len);

  ~AudioConverter();
  const AVCodec*      m_codec         = nullptr;

private:
  AVCodecContext*     m_codec_context = nullptr;
  AVFrame*            m_frame         = nullptr;
  AVFrame*            m_flush_frame   = nullptr;
  AVPacket*           m_packet        = nullptr;
  uint16_t            m_sample        = 0;
  bool                m_valid         = true;
  bool                m_awaiting      = false;

  void encode(AVData &t_avdata, bool t_flush = false);

  void set_channel_layout();
  void setup_frame();
  void copy_to_frame(uint8_t *t_audio_buffer, std::size_t t_frame_size_bytes, std::size_t t_offset);
  void write_zeros_to_frame(std::size_t t_frame_size_bytes);

  bool validate_sample_format();
  bool validate_sample_rate();
  
  template<typename T>
  bool is_valid_pointer(T t_pointer, const char *t_msg);
  bool is_valid(int t_result, const char *t_msg);


  static AudioSettings *m_SETTINGS;

};

#endif
