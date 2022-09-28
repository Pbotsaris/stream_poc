#ifndef AV_OUTPUT_STREAM_H
#define AV_OUTPUT_STREAM_H

extern "C" { // must use to link
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

struct AudioSettings {

 private:
  int m_channels = 1;
  int m_bitrate = 6400;
  int m_samplerate = 44100;
  AVSampleFormat m_sample_format = AV_SAMPLE_FMT_S16; // bit depth int 16 bits

public:

  int bitrate() const { return m_bitrate; }
  int samplerate() const { return m_samplerate; }
  int channels() const { return m_channels; }
  AVSampleFormat sample_format() const { return m_sample_format ;}
};


struct VideoSettings {

private:
  int m_bitrate = 40000;
  int m_height = 720;
  int m_width = 1280;
  int m_framerate = 25;

public:

  int bitrate() const { return m_bitrate; }
  int width() const { return m_width; }
  int framerate() const { return m_framerate; }
  int height() const { return m_height; }
};

struct AVOutputStream {
    AVStream*             stream        = nullptr;
    const AVCodec*        codec         = nullptr;
    AVCodecContext*       codec_context = nullptr;
    
    /* pts of the next frame that will be generated */
    int64_t               next_frame;
    int                   num_samples;

    AVFrame*              frame = nullptr;
    AVFrame*              frame_temp = nullptr;

    // Resampler context 
    struct SwsContext*    sws_context = nullptr;
    struct SwrContext*    resampler_context = nullptr;

    AVPacket*             packets = nullptr;
    bool                  m_valid = true;

    bool valid() const;
    int codec_type() const;

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


    void set_audio_settings(AudioSettings &t_settings);
    void set_video_settings(VideoSettings &t_settings, AVCodecID t_codec_id);

    ~AVOutputStream();

  private:

  template <typename T>
  bool is_valid_pointer(T t_pointer, const char *t_msg);

  bool is_valid(int t_result, const char *t_msg);

  inline static const int m_DEFAULT_SAMPLE_NUMBER = 1000;

  void alloc_audio_frame(AVFrame *t_frame, AVSampleFormat t_sample_format);

  int number_audio_samples() const;


};

#endif
