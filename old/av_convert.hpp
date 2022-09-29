#ifndef AVCONVERT_H
#define AVCONVERT_H

extern "C" { // must use to link
            
#include <libavutil/avassert.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>

}

#include <string>

#include "av_output_stream.hpp"

class AVConvert {

public:
  AVConvert(const char *t_format, AudioSettings &&t_audio_settings,
            VideoSettings &&t_video_settings);

  ~AVConvert();

  void create_streams();
  void open_audio();
  void open_video();
  void encode(double t_duration); // duration to be encoded

private:
  AudioSettings m_audio_settings;
  VideoSettings m_video_settings;
  AVOutputStream *m_video_stream;
  AVOutputStream *m_audio_stream;

  AVDictionary *m_options = nullptr;

  AVFormatContext *m_format_context = nullptr;
  const AVOutputFormat *m_output_format = nullptr;

  bool m_valid = true;

  /* methods */
  void create_stream(AVOutputStream *t_stream, AVCodecID t_codec_id);
  void set_options(AVOutputStream *t_stream);
  void stream_params_to_muxer(AVOutputStream *t_stream);
  bool next_frame_ready();
  bool has_ended(AVOutputStream *t_stream, double t_duration);

  bool is_invalid(int t_result, const char *t_msg);
  bool is_invalid_codec();

  template <typename T> bool is_invalid_pointer(T t_pointer);
};

#endif
