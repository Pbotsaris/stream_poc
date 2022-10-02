#ifndef AUDIO_VIDEO_STREAMS_H
#define AUDIO_VIDEO_STREAMS_H
#include "SDL_audio.h"
#include "audio_config.hpp"
#include "audio_context.hpp"
#include "av_data.hpp"
#include "udp_conn.hpp"
#include "webcam.hpp"
#include "audio_context.hpp"

class AudioVideoStreams{

  public:
  AudioVideoStreams(AVData &t_data);
  ~AudioVideoStreams();

  void capture();

  private:
  AudioContext m_audio;
  Webcam m_webcam;      

  static VideoSettings *m_VIDEO_SETTINGS;
};












#endif

