#include <iostream>
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <memory>
//#include "audio_video_streams.hpp"
#include "audio_converter.hpp"
#include "audio_device.hpp"
#include "audio_device_config.hpp"
#include "lock_free_audio_queue.hpp"

void free_configs(){

  AudioDevConfig *config = AudioDevConfig::get_instance();
  AudioSettings *audio_settings = AudioSettings::get_instance();
  VideoSettings *video_settings = VideoSettings::get_instance();

  config->delete_instance();
  audio_settings->delete_instance();
  video_settings->delete_instance();

}

int main(void) {

  SDL_Init(SDL_INIT_AUDIO);

  auto input_queue = std::make_unique<LockFreeAudioQueue>();
  auto output_queue = std::make_unique<LockFreeAudioQueue>();

  AudioDevice input_device(input_queue, AudioDevice::Input);
  AudioDevice output_device(output_queue, AudioDevice::Output);

  auto converter = AudioConverter();

  input_device.open();
  input_device.wait(40); // wait for 40 frames just to buffer the queue with some audio data.

  // we don't want the audio to stream forever for this test so we will quick after a certain number of iterations
  int safe_count = 0;


  // if the input queue is empty just quit.
  while (!input_queue->empty()) {

    // when can check if the converter has broken and exit the loop
    if(!converter.valid()){
      std::cout << "Converter not valid\n";
      break;
    }

   // This encode call pulls and converts the exact amount of frames defined in
   // VideoSettings::m_capture_size; so if m_capture_size = 2 the converter will
   // convert 2 frames audio per encode() call.
    std::vector<uint8_t> encoded_audio = converter.encode(input_queue); // encoding from input_queue;
    
  
    //  The decoder will convert whatever buffer you give to it. It will split the converted output 
    //  in chunks with exact buffer size defined in AudioSettings::buffersize (that the audio callback is also using)
    //  It will then push these packets to the output queue being read by the audio callback on another thread to output to the user speakers.
    converter.decode(output_queue, encoded_audio); 

    // Let's check if our output queue got any decoded packages by popping the first audio package from it.
    if (!output_queue->empty()) {
      auto some_audio_data = output_queue->pop();
      std::cout << " data package lenght: -> " << some_audio_data->m_len << "\n";
    } else {
      std::cout << "output is still queue is empty.\n";
    }

    // let's quit after 10 iterations
    if (safe_count > 10) {
     input_device.close();
      break;
    }

    safe_count++;
  }

  free_configs();
  return 0;
}
