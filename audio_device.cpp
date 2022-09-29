//#include "audio_device.hpp"
//
//
//AudioDevice::AudioDevice(std::size_t t_buffer_size)
//    :  m_status(Closed) {
//
//  SDL_AudioSpec want, have;
//  SDL_zero(want);
//  want.freq = 44100;
//  want.format = AUDIO_S16; // signed 16-bit samples in little-endian byte order 16bit, 32 float,
//  want.channels = 1;
//  want.samples = t_buffer_size; // based on 25fps
//  want.callback = audio_recording_callback;
//
//  want.userdata = this;
//  m_dev = SDL_OpenAudioDevice(m_CONFIG->get_input().c_str(), INPUT_AUDIO_DEVICES, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
//
//  if (m_dev == 0) {
//
//    SDL_Log("Failed to open audio: %s", SDL_GetError());
//    m_status = Invalid;
//  };
//
//  m_byte_per_sample = have.channels * (SDL_AUDIO_BITSIZE(have.format) / 8);
//  m_byte_per_second = have.freq * m_byte_per_sample;
//}
//
//void AudioDevice::open(std::size_t t_time) {
//  if (m_status == Opened) {
//    // TODO: Log error
//    return;
//  }
//
//  SDL_PauseAudioDevice(m_dev, 0);
// // m_status = Opened;
//  SDL_Delay(t_time);
//  SDL_PauseAudioDevice(m_dev, 1);
//
//  m_status = Closed;
//}
//
//// 16 bit 44100    | 25 frames
//// 44100 * (1/25) * 2(1 sample = 2 bytes) = 3528
//
//void AudioDevice::audio_recording_callback(void *user_data, Uint8 *stream, int len) {
//  std::cout << len << "\n";
//  // m_conn.send_out(std::string("127.0.0.1"), std::string("65393"), stream,
//  // len);
//};
