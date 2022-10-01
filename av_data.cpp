#include "av_data.hpp"
#include "av_settings.hpp"

AVData::AVData() {
  AudioSettings *audio_settings = AudioSettings::get_instance();
  VideoSettings *video_settings = VideoSettings::get_instance();

  m_max_audio_size =
      audio_settings->buffer_size() * video_settings->buffer_size_in_frames();
  m_max_video_size = video_settings->buffer_size();

  m_audio_buffer = new uint8_t[m_max_audio_size];
};

void AVData::load_audio(uint8_t *t_audio_data, std::size_t t_size) {
  if (m_audio_size < m_max_audio_size) {

    for (std::size_t i = 0; i < t_size; i++) {
      m_audio_buffer[m_audio_size + i] = t_audio_data[i];
      m_audio_size++;
    }
  } else {
    std::cout << "Error: Audio buffer is full.\n";
  }
}

std::size_t AVData::audio_size() const { return m_audio_size; }

void AVData::load_video(){};

void AVData::reset_audio_buffer() { m_audio_size = 0; }

AVData::~AVData() {
  delete[] m_audio_buffer;
  // delete[] m_data;
}
