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

#include <stdlib.h>

void free_configs() {

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

  AudioDevice input_device(output_queue, AudioDevice::Input);
  AudioDevice output_device(output_queue, AudioDevice::Output);

  auto converter = AudioConverter();

  std::cout << "Recording. Say something.\n";

  input_device.open();
  input_device.wait(100); // record for 100 frames!
  input_device.close();

  std::cout << "Converting....\n";

  while (!input_queue->empty()) {

    if (!converter.valid()) {
      std::cout << "Converter not valid\n";
      break;
    }

    std::vector<uint8_t> encoded_audio = converter.encode(input_queue);
    converter.decode(output_queue, encoded_audio);
  }

  std::cout << "done with input! Now playing the ouput of the recorded audio.\n";

  output_device.open();
  output_device.wait(100);
  output_device.close();

  SDL_Quit();
  free_configs();
  return 0;
}
