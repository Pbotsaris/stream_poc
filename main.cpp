#include <iostream>
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <memory>
//#include "audio_video_streams.hpp"
#include "audio_converter.hpp"
#include "audio_device.hpp"
#include "lock_free_audio_queue.hpp"

int main(void) {

  SDL_Init(SDL_INIT_AUDIO);

  auto input_queue = std::make_unique<LockFreeAudioQueue>();
  auto output_queue = std::make_unique<LockFreeAudioQueue>();

  AudioDevice device(input_queue);
  auto converter = AudioConverter();

  device.open();
  device.wait(40); // wait 10 frames just to buffer the queue with some audio data.

  // we don't want the audio to stream forever for this test so we will quick after a certain number of iterations
  int safe_count = 0;


  // if the input queue is empty just quit.
  while (!input_queue->empty()) {

    // when can check if the converter has broken and exit the loop
    if(!converter.valid()){
      std::cout << "Converter not valid\n";
      break;
    }

   // This encode call pull and convert the exact amount of frames is
   // VideoSettings::m_capture_size; so if m_capture_size = 2 the converter will
   // convert 2 frames audio per call.
    std::vector<uint8_t> encoded_audio = converter.encode(input_queue); // encoding from input_queue;
    
  
    // let's decode what we just encoded and push it to the output_queue (this will feed the output audio callback).
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
     device.close();
      break;
    }

    safe_count++;
  }

  return 0;
}
