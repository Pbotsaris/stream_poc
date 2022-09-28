#ifndef AV_SETTINGS_H
#define AV_SETTINGS_H

extern "C" {
#include <libavformat/avformat.h>
}

struct AudioSettings {

 private:
  int m_channels = 1;
  int m_bitrate = 64000;
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
  int m_height = 480;
  int m_width = 640;
  int m_framerate = 25;

public:
 
  VideoSettings(){};

  VideoSettings(int t_bitrate, int t_height, int t_width, int t_framerate):
    m_bitrate(t_bitrate), m_height(t_height), m_width(t_width), m_framerate(t_framerate){};

  VideoSettings(int t_height, int t_width, int t_framerate):
    m_height(t_height), m_width(t_width), m_framerate(t_framerate){};

 VideoSettings(int t_height, int t_width):
    m_height(t_height), m_width(t_width){};


  int bitrate() const { return m_bitrate; }
  int width() const { return m_width; }
  int framerate() const { return m_framerate; }
  int height() const { return m_height; }
};

#endif
