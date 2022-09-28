#ifndef AV_OUTPUT_STREAM_H
#define AV_OUTPUT_STREAM_H

extern "C" { // must use to link
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

#include "av_settings.hpp"

struct AVOutputStream {
    AVStream*             stream               = nullptr;
    const AVCodec*        codec                = nullptr;
    AVCodecContext*       codec_context        = nullptr;
    
    /* pts of the next frame that will be generated */
    int64_t               next_frame_position  = 0;
    int                   num_samples          = 0;

    AVFrame*              frame               = nullptr;
    AVFrame*              frame_temp          = nullptr;

    // Resampler context 
    struct SwsContext*    conversion_context  = nullptr; // video conversion
    struct SwrContext*    resampler_context   = nullptr; // audio resampler

    AVPacket*             packets             = nullptr;
    bool                  m_valid             = true;

    bool valid() const;
    int  codec_type() const;

    void find_codec(AVCodecID t_codec_id);
    void new_stream(AVFormatContext *t_format_context);
    void alloc_packets();
    void alloc_codec_context();
    void set_stream_id(int t_id);
    void set_sample_format();
    void alloc_audio_frames(AVSampleFormat t_sample_format);
    void alloc_video_frame();
    void alloc_resampler_context();
    void init_resampler_context(AVSampleFormat t_sample_format);
    void init_conversion_context();
    void set_audio_settings(AudioSettings &t_settings);
    void set_video_settings(VideoSettings &t_settings, AVCodecID t_codec_id);

    ~AVOutputStream();

  /*******  Private   ***********/
  private:
  template <typename T>
  bool is_valid_pointer(T t_pointer, const char *t_msg);

  bool is_valid(int t_result, const char *t_msg);

  inline static const int m_DEFAULT_SAMPLE_NUMBER = 1000;
  inline static const int m_CONVERSION_SCALE_FLAGS = SWS_BICUBIC;

  void alloc_audio_frame(AVFrame *t_frame, AVSampleFormat t_sample_format);

  int number_audio_samples() const;
};

#endif
