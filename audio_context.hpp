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

  AudioContext(AudioDevConfig *config, AVData &t_data);
  void capture(std::size_t t_nb_frame);

  protected:
  void countdown(std::size_t t_read_size);
  void copy_audio_data(Uint8 *t_stream, int len);

  private:
    SDL_AudioDeviceID m_dev        = 0;
    AudioConverter    m_converter;
    Status            m_status     = Closed;
    std::size_t       m_read_size  = 0;
    AVData            &m_data;

    static void audio_callback(void *user_data, Uint8 *stream, int len);
    static AudioSettings *m_AUDIO_SETTINGS;
    static VideoSettings *m_VIDEO_SETTINGS;
};

#endif
