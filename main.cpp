#include <iostream>
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <memory>
//#include "audio_video_streams.hpp"
#include "audio_context.hpp"
#include "lock_free_audio_queue.hpp"

int main(void) {

//  auto queue = LockFreeAudioQueue();
//
//  auto data = new uint8_t[2];
//  auto data2 = new uint8_t[2];
//  auto data3 = new uint8_t[2];
//
//  data[0] = 'p';
//  data[1] = 'e';
//
//  data2[0] = 'd';
//  data3[1] = '1';
//
//
//  queue.push(AudioPackage(data, 2));
//  queue.push(AudioPackage(data2, 2));
//  queue.push(AudioPackage(data3, 2));
//
//  int i = 0;
//  while(!queue.empty()){
//
//    queue.pop();
//
//    i++;
//
//    if(i > 10){
//      break;
//    }
//  }
//
//  delete [] data;
//  delete [] data2;
//  delete [] data3;
//
   SDL_Init(SDL_INIT_AUDIO);
  
    auto audio_queue = std::make_unique<LockFreeAudioQueue>();
  
    int i = 0;
    int max_reads = 5;
  
    AudioDevice m_device(audio_queue);
  
    m_device.open();
    m_device.wait(15);
  
    while(!audio_queue->empty()){
    auto package = audio_queue->pop();
    std::cout << "length: " << package->m_len << std::endl;
  // converting to int16_t because this is usingned 16 bit audio.
  //  doing this just to test printing the audio data.
     uint16_t *sample = (uint16_t*) package->m_data;
     std::cout << "sample [0]: " << sample[0] << "\n";
     std::cout << "sample [1]: " << sample[1] << "\n";
     m_device.wait(3);
     i++;
  
    std::cout << "length: " << package->m_len << std::endl;
    std::cout << "read number: " << i << "\n";
  
     if(i == max_reads){ // reading 10 packages than stop audio callback.
    //   m_device.close();
     }
  
     if(i == (max_reads + 20)){
       break;
     }
    }

    m_device.close();
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
  //  std::size_t size =  audio_settings->buffer_size() *
  //  video_settings->buffer_size_in_frames();
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
