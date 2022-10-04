#include <iostream>
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <memory>
//#include "audio_video_streams.hpp"
#include "audio_device.hpp"
#include "lock_free_audio_queue.hpp"

int main(void) {

  SDL_Init(SDL_INIT_AUDIO);

  auto audio_queue = std::make_unique<LockFreeAudioQueue>();

  int i = 0;
  int max_reads = 10;

  AudioDevice m_device(audio_queue);

  m_device.open();
  m_device.wait(10); // wait 10 frames to buffer que queue

  while (true) {

    auto package = audio_queue->pop();
    std::cout << "length: " << package->m_len << std::endl;
    // converting to int16_t because this is usingned 16 bit audio.
    //  doing this just to test printing the audio data.
    uint16_t *sample = (uint16_t *)package->m_data;
    std::cout << "sample [0]: " << sample[0] << "\n";
    std::cout << "sample [1]: " << sample[1] << "\n";
    std::cout << "sample length: " << package->m_len << std::endl;
    std::cout << " number of reads: " << i << "\n";

    // if the queue is empty wait for more packages
    if (audio_queue->empty()) {
      std::cout << "waiting 10 frames ...\n";
      m_device.wait(10);
    }

    if (i == max_reads) { //  close after a certain number of reads
      std::cout << "closing" << std::endl;
      m_device.close();

      // empty the queue before braking
      while (audio_queue->empty()) {
        auto pack = audio_queue->pop();
        // we could print this package here to but we won't :)
      };

      break; // break from main loop
    }

    i++;
  }

  return 0;
}
