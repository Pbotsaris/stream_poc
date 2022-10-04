#include "audio_context.hpp"

AudioSettings *AudioContext::m_AUDIO_SETTINGS = AudioSettings::get_instance();
VideoSettings *AudioContext::m_VIDEO_SETTINGS = VideoSettings::get_instance();

AudioContext::AudioContext(AVData &t_data) : m_data(t_data) {
  SDL_AudioSpec want, have;

  AudioDevConfig *config = AudioDevConfig::get_instance();

  want.freq = m_AUDIO_SETTINGS->samplerate();
  SDL_zero(want);
  want.format = AUDIO_S16; // signed 16-bit samples in little-endian byte order
                           // 16bit, 32 float,
  want.channels = m_AUDIO_SETTINGS->channels();
  want.callback = audio_callback;
  want.samples = m_AUDIO_SETTINGS ->buffer_size_in_samples(); // 1 video frame of audio in SAMPLES based on 25fps.
  want.userdata = this;

  m_dev = SDL_OpenAudioDevice(config->get_input().c_str(), INPUT_AUDIO_DEVICES,
                              &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

  if (m_dev == 0) {
    SDL_Log("Failed to open audio: %s", SDL_GetError());
    m_status = Invalid;
  };

  log_on_mismatch_audiospec(want, have);
}

void AudioContext::capture() {

  int nb_frames_to_capture = m_VIDEO_SETTINGS->buffer_size_in_frames();
  m_done = false;

  // we set our audio capture size to the number of frames we will capture in the video
  m_read_size = nb_frames_to_capture * m_AUDIO_SETTINGS->buffer_size();
  SDL_PauseAudioDevice(m_dev, 0); // will close from the callback;

  while (!m_done) { // while not done
    SDL_Delay(1000 / m_VIDEO_SETTINGS->framerate() * nb_frames_to_capture);
    if (!m_converter.valid()) {
      break;
    }
  }
};

void AudioContext::countdown(std::size_t t_read_size) {

  if (m_read_size == 0) {
    std::cout << "Closing audio device...\n" << std::endl;
    m_done = true;
    SDL_PauseAudioDevice(m_dev, 1); // close audio on complete;
  }


  if (!m_done && t_read_size > m_read_size) {
    // TODO: Log error
    std::cout << "requested audio read and read size did not match. was: " << t_read_size << " should be: " << m_read_size << "\n";
    m_read_size = 0;
    m_done = true;
    SDL_PauseAudioDevice(m_dev, 1); // close callback on error;
  }

  m_read_size -= t_read_size;

  std::cout << "readsize = " << m_read_size << "\n";
}

void AudioContext::encode(Uint8 *t_stream, int len) {

  if (!m_done && m_converter.valid()) {
    m_converter.encode(m_data, t_stream, len);
  }
}

void AudioContext::log_on_mismatch_audiospec(SDL_AudioSpec t_want,
                                             SDL_AudioSpec t_have) {

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

void AudioContext::audio_callback(void *user_data, Uint8 *stream, int len) {
  AudioContext *ac = (AudioContext *)user_data;

  ac->countdown(len);
  ac->encode(stream, len);
};
