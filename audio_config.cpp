#include "audio_config.hpp"
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_types.h>
#include <iostream>

AudioConfig *AudioConfig::m_instance = nullptr;

AudioConfig *AudioConfig::get_instance(){
  if(m_instance == nullptr){
    m_instance = new AudioConfig();
  }

 return m_instance;
}

void AudioConfig::delete_instance(){
  delete m_instance;
}

AudioConfig::AudioConfig() {
  get_device_list();
  select_device(0, Input);
  select_device(0, Output);
}

void AudioConfig::select_input(std::size_t t_pos) { select_device(t_pos, Input); }
void AudioConfig::select_output(std::size_t t_pos) { select_device(t_pos, Output); }

std::string AudioConfig::get_input() const { return m_selected_input; }
std::string AudioConfig::get_output() const { return m_selected_output; }

AudioConfig::DeviceListConst AudioConfig::list_input_name() const { return m_input_devs; }
AudioConfig::DeviceListConst AudioConfig::list_output_name() const { return m_output_devs; }

std::size_t AudioConfig::input_count() const { return m_input_devs.size(); }
std::size_t AudioConfig::output_count() const { return m_output_devs.size(); }

void AudioConfig::get_device_list() {
  int dev_count = SDL_GetNumAudioDevices(0);
  for (int i = 0; i < dev_count; ++i) {
    m_input_devs.push_back(SDL_GetAudioDeviceName(i, m_INPUT_DEV));
    m_output_devs.push_back(SDL_GetAudioDeviceName(i, m_OUTPUT_DEV));
  }
}

void AudioConfig::select_device(std::size_t t_pos, DevType m_type) {
  if (t_pos < m_input_devs.size()) {
    switch (m_type) {
    case Input:
      m_selected_input = m_input_devs.at(t_pos);
    case Output:
      m_selected_output = m_output_devs.at(t_pos);
    }
  } else {
    std::cout << "Error: Could not select input. Bad position." << std::endl;
  }
}
