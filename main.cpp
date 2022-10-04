#include <iostream>
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <memory>
//#include "audio_video_streams.hpp"
#include "audio_device.hpp"
#include "audio_converter.hpp" 
#include "lock_free_audio_queue.hpp"

int main(void) {

  SDL_Init(SDL_INIT_AUDIO);

  auto audio_queue = std::make_unique<LockFreeAudioQueue>();
  AudioDevice device(audio_queue);
  auto converter = AudioConverter();

  device.open();
  device.wait(10); // wait 10 frames just to buffer the queue some audio.

  // This convert call pull and convert the exact amount of frames is VideoSettings::m_capture_size;
  // so if m_capture_size = 2 the converter will convert 2 frames audio per call.
  std::vector<uint8_t> result = converter.encode(audio_queue); 

  // printing the size of the buffer captured and converted.
  device.close();
  std::cout << result.size();
  return 0;
}
