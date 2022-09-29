#ifndef AUDIO_CONTEXT_H
#define AUDIO_CONTEXT_H

#define INPUT_AUDIO_DEVICES 1
#define OUTPUT_AUDIO_DEVICES 0


#include "udp_conn.hpp"
#include "audio_config.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_types.h>
#include <functional>


#include "audio_converter.hpp"
#include "audio_config.hpp"
#include "av_settings.hpp"

class AudioContext {

  public:
  enum Status { Invalid, Closed, Opened };

  AudioContext(AudioDevConfig *config);

  void capture(AVData &t_data, std::size_t t_nb_frame);

  void test() { std::cout << "test\n";};
  void countdown(std::size_t t_read_size);

  private:

    SDL_AudioDeviceID m_dev;
    AudioConverter    m_converter;
    Status            m_status;
    std::size_t       m_read_size;


    void close();
    static void audio_callback(void *user_data, Uint8 *stream, int len);
    static AudioSettings *m_SETTINGS;
};


#endif
