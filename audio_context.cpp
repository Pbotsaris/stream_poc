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
  want.samples = m_AUDIO_SETTINGS->buffer_size(); // based on 25fps
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

void AudioContext::capture() {

  int nb_frames_to_capture = m_VIDEO_SETTINGS->buffer_size_in_frames();
  // 25 is the frame rate
  m_read_size = nb_frames_to_capture * m_AUDIO_SETTINGS->buffer_size();
  SDL_PauseAudioDevice(m_dev, 0); // will close from the callback;

  while (m_read_size != 0) {
    SDL_Delay(1000 / m_VIDEO_SETTINGS->framerate() * nb_frames_to_capture);
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
    std::cout << "Closing audio device...\n" << std::endl;
    SDL_PauseAudioDevice(m_dev, 1); // close audio on complete;
  }
}

void AudioContext::encode(Uint8 *t_stream, int len) {

  if (m_read_size != 0) {
    m_converter.encode(m_data, t_stream, len);
    // m_data.load_audio(static_cast<uint8_t *>(t_stream), len);
  }
}

void AudioContext::audio_callback(void *user_data, Uint8 *stream, int len) {
  AudioContext *ac = (AudioContext *)user_data;

  ac->countdown(len);
  ac->encode(stream, len);
};
