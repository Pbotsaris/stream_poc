#include "audio_device.hpp"

AudioDevice::AudioDevice(AudioConfig *t_config) : m_config(t_config){

    SDL_AudioSpec want, have;
    SDL_zero(want);
    want.freq = 44100; 
    want.format = AUDIO_S16; // signed 16-bit samples in little-endian byte order  16bit, 32 float,  
    want.channels = 1;
    want.samples = 4096;
    want.callback = audio_recording_callback;

    m_dev =
        SDL_OpenAudioDevice(m_config->get_input().c_str(), INPUT_AUDIO_DEVICES,
                            &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    if (m_dev == 0) {
      SDL_Log("Failed to open audio: %s", SDL_GetError());
      m_status = Invalid;
    };

    m_byte_per_sample = have.channels * (SDL_AUDIO_BITSIZE(have.format) / 8);
    m_byte_per_second = have.freq * m_byte_per_sample;

    SDL_PauseAudioDevice(m_dev, 0);
    SDL_Delay(1000);
    SDL_PauseAudioDevice(m_dev, 1);

    m_status = Opened;

}

 void AudioDevice::audio_recording_callback(void *user_data, Uint8 *stream, int len) {
    m_conn.send_out(std::string("127.0.0.1"), std::string("65393"), stream, len);
  };
