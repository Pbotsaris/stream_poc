#include "audio_context.hpp"

AudioSettings *AudioDevice::m_AUDIO_SETTINGS = AudioSettings::get_instance();
VideoSettings *AudioDevice::m_VIDEO_SETTINGS = VideoSettings::get_instance();

AudioDevice::AudioDevice(std::unique_ptr<LockFreeAudioQueue> &t_queue) {
  SDL_AudioSpec want, have;

  AudioDevConfig *config = AudioDevConfig::get_instance();

  want.freq = m_AUDIO_SETTINGS->samplerate();
  SDL_zero(want);
  want.format = AUDIO_S16; // signed 16-bit samples in little-endian byte order.
  want.channels = m_AUDIO_SETTINGS->channels();
  want.callback = audio_callback;
  want.samples = m_AUDIO_SETTINGS ->buffer_size_in_samples(); // 1 video frame of audio in SAMPLES based on 25fps.
  want.userdata = t_queue.get();

  m_dev = SDL_OpenAudioDevice(config->get_input().c_str(), INPUT_AUDIO_DEVICES,
                              &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

  if (m_dev == 0) {
    m_status = Invalid;
    SDL_Log("Failed to open audio: %s", SDL_GetError());
  };

  log_on_mismatch_audiospec(want, have);
}

void AudioDevice::open() {
  if(m_status == Closed){ // TODO: Log
  SDL_PauseAudioDevice(m_dev, 0); 
   m_status = Opened;
  }
};

void AudioDevice::close() {
  if(m_status == Opened){ // TODO: Log
    SDL_Log("closing!...\n");
  SDL_PauseAudioDevice(m_dev, 1); 
   m_status = Closed;
  }
};

void AudioDevice::wait(int t_frames) {
SDL_Delay(1000/m_VIDEO_SETTINGS->framerate() * t_frames);
};


void AudioDevice::log_on_mismatch_audiospec(SDL_AudioSpec t_want, SDL_AudioSpec t_have) {

  if (t_have.channels != t_want.channels) {
    SDL_Log("Audio spec didn't match. Want: %i have: %i\n", t_want.channels,
            t_have.channels);
  }

  if (t_have.format != t_want.format) {
    SDL_Log("Format didn't match. Want: %i have: %i\n", t_want.format,
            t_have.format);
  }

  if (t_have.samples != t_want.samples) {
    SDL_Log("Buffer size didn't match. Want: %i have: %i\n", t_want.samples,
            t_have.samples);
  }
}

void AudioDevice::audio_callback(void *user_data, Uint8 *stream, int len) {
  
  LockFreeAudioQueue *queue = (LockFreeAudioQueue *)user_data;
   queue->push(AudioPackage(stream, len));
};
