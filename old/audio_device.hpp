//
//#ifndef AUDIO_DEVICE_H
//#define AUDIO_DEVICE_H
//
//#include "audio_config.hpp"
//#include "udp_conn.hpp"
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_audio.h>
//#include <SDL2/SDL_types.h>
//#include <functional>
//
//#define INPUT_AUDIO_DEVICES 1
//#define OUTPUT_AUDIO_DEVICES 0
//
//class AudioDevice {
//public:
//  enum Status { Invalid, Closed, Opened };
//
//  AudioDevice(std::size_t t_buffer_size);
//
//  void open(std::size_t t_time);
//
//  void test() { std::cout << "test\n" ;}
//
//private:
//  SDL_AudioDeviceID m_dev;
//
//  inline static UDPConn m_conn = UDPConn(6000, std::string("127.0.0.1"));
//
//  Status m_status;
//  int m_byte_per_sample;
//  int m_byte_per_second;
//
//  static void audio_recording_callback(void *user_data, Uint8 *stream, int len);
//
//};
//
//#endif
