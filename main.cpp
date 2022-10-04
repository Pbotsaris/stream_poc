#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <memory>
#include "audio_video_streams.hpp"
#include "lock_free_audio_queue.hpp"

int main(void) {
 
   
  LockFreeAudioQueue audio_queue;

  uint8_t *data = new uint8_t[2];

  data[0] = 'a';
  data[1] = 'b';

  AudioPackage package(data, 2);

  std::cout << "printing before...\n";
  std::cout << package.m_data[0] << std::endl;

  audio_queue.push(std::move(package));


  std::shared_ptr<AudioPackage> res = audio_queue.pop();

  std::cout << "printing...\n";
  std::cout << res->m_data[0] << std::endl;
  std::cout << res->m_len << std::endl;

  delete [] data;
//  SDL_Init(SDL_INIT_AUDIO);
//
//  VideoSettings *video_settings = VideoSettings::get_instance();
//  AudioSettings *audio_settings = AudioSettings::get_instance();
//
//  auto data = AVData();
//  auto audio = AudioContext(data);
//
//  audio.capture();
//
//  std::size_t size =  audio_settings->buffer_size() * video_settings->buffer_size_in_frames();
//
//  std::cout << "was size: " << size << "\n";
//
//  std::cout << "compressed size:" << data.audio_size() << "\n";
//
//  AudioSettings::delete_instance();
//  AudioDevConfig::delete_instance();
//  VideoSettings::delete_instance();


  return 0;
}
