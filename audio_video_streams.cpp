#include "audio_video_streams.hpp"

VideoSettings* AudioVideoStreams::m_VIDEO_SETTINGS = VideoSettings::get_instance();

AudioVideoStreams::AudioVideoStreams(AVData &t_data)
    : m_audio(AudioContext(t_data)), m_webcam() {

  SDL_Init(SDL_INIT_AUDIO);
}


void AudioVideoStreams::capture(){
   m_audio.capture();
}

AudioVideoStreams::~AudioVideoStreams() {

  AudioSettings::delete_instance();
  AudioDevConfig::delete_instance();
  VideoSettings::delete_instance();
}
