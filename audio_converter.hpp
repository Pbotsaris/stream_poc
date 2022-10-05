#ifndef AUDIO_CONVERTER_H
#define AUDIO_CONVERTER_H

extern "C" { // must use to link
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/frame.h>
#include <libavutil/samplefmt.h>
}

#include <vector>

#include "av_settings.hpp"
#include "av_data.hpp"
#include "lock_free_audio_queue.hpp"

class AudioConverter {

public:
  AudioConverter();
  std::size_t frame_size_bytes() const;
  std::size_t frame_size_samples() const;
  bool valid() const;

  std::vector<uint8_t> encode(std::unique_ptr<LockFreeAudioQueue> &t_queue);

  ~AudioConverter();
  const AVCodec*      m_codec         = nullptr;

private:
  AVCodecContext*     m_codec_context = nullptr;
  AVFrame*            m_frame         = nullptr;
  AVFrame*            m_flush_frame   = nullptr;
  AVPacket*           m_packet        = nullptr;
  bool                m_valid         = true;
  bool                m_awaiting      = false;


  void encode_package(std::shared_ptr<AudioPackage> &t_package, std::vector<uint8_t> &t_data);
  void encode_frames(std::vector<uint8_t> &t_data, bool t_flush = false);

  void set_channel_layout();
  void setup_frame();
  void copy_to_frame(uint8_t *t_audio_buffer, std::size_t t_frame_size_bytes, std::size_t t_offset);
  void write_zeros_to_frame(std::size_t t_frame_size_bytes, std::size_t t_offset);

  bool validate_sample_format();
  bool validate_sample_rate();
  
  template<typename T>
  bool is_valid_pointer(T t_pointer, const char *t_msg);
  bool is_valid(int t_result, const char *t_msg);


  static AudioSettings *m_AUDIO_SETTINGS;
  static VideoSettings *m_VIDEO_SETTINGS;

};

#endif
