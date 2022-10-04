#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <memory>
#include "audio_video_streams.hpp"

int main(void) {

  SDL_Init(SDL_INIT_AUDIO);

  VideoSettings *video_settings = VideoSettings::get_instance();
  AudioSettings *audio_settings = AudioSettings::get_instance();

  auto data = AVData();
  auto audio = AudioContext(data);

  audio.capture();

  std::size_t size =  audio_settings->buffer_size() * video_settings->buffer_size_in_frames();

  std::cout << "was size: " << size << "\n";

  std::cout << "compressed size:" << data.audio_size() << "\n";

  AudioSettings::delete_instance();
  AudioDevConfig::delete_instance();
  VideoSettings::delete_instance();


  return 0;
}
