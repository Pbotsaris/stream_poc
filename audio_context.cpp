#include "audio_context.hpp"

AudioSettings* AudioContext::m_SETTINGS = AudioSettings::get_instance();

AudioContext::AudioContext(AudioDevConfig *config)
    : m_dev(0), m_converter(),
      m_status(Closed), m_read_size(0) {
  SDL_AudioSpec want, have;

  want.freq = m_SETTINGS->samplerate();
  SDL_zero(want);
  want.format = AUDIO_S16; // signed 16-bit samples in little-endian byte order
                           // 16bit, 32 float,
  want.channels = m_SETTINGS->channels();
  want.samples = m_SETTINGS->buffer_size(); // based on 25fps
  want.callback = audio_callback;
  want.userdata = this;

  m_dev = SDL_OpenAudioDevice(config->get_input().c_str(), INPUT_AUDIO_DEVICES,
                              &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

  if (m_dev == 0) {

    SDL_Log("Failed to open audio: %s", SDL_GetError());
    m_status = Invalid;
  };
}

// TODO: YOU DONE CAPTURE NOW GO COMPRESS!';///// '

void AudioContext::capture(AVData &t_data, std::size_t t_nb_frames) {

  // 25 is the frame rate
  m_read_size = t_nb_frames * m_SETTINGS->buffer_size();
  SDL_PauseAudioDevice(m_dev, 0); // will close from the callback;

  while (m_read_size != 0) {
    SDL_Delay(1000 / 25 * t_nb_frames);
  }
};

void AudioContext::countdown(std::size_t t_read_size) {

  if (t_read_size > m_read_size) {
    // TODO: Log error
    std::cout << "requested audio read and read size did not match. was"
              << t_read_size << "should be: " << m_read_size;

    m_read_size = 0;
    SDL_PauseAudioDevice(m_dev, 1); // close callback on error;
  }

  m_read_size -= t_read_size;

  if (m_read_size == 0) {
    std::cout << "Closed device\n" << std::endl;
    SDL_PauseAudioDevice(m_dev, 1); // close audio on complete;
  }
}

void AudioContext::audio_callback(void *user_data, Uint8 *stream, int len) {
  AudioContext *ac = (AudioContext *)user_data;

  ac->countdown(len);
};

void AudioContext::close() { SDL_PauseAudioDevice(m_dev, 1); }
