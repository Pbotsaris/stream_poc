#include "av_data.hpp"
#include "av_settings.hpp"

AVData::AVData() {
  AudioSettings *audio_settings = AudioSettings::get_instance();
  VideoSettings *video_settings = VideoSettings::get_instance();

  m_audio_size =
      audio_settings->buffer_size() * video_settings->buffer_size_in_frames();
  m_video_size = video_settings->buffer_size();

  m_size = m_audio_size + m_video_size;

  m_audio_buffer = new uint8_t[m_audio_size];
};

void AVData::load_audio(uint8_t *t_audio_data, std::size_t t_size) {
  if (m_audio_index < m_audio_size) {

    for (std::size_t i = 0; i < t_size; i++) {
      m_audio_buffer[m_audio_index + i] = t_audio_data[i];
      m_audio_index++;
    }
  } else {
    // TODO: LOG
    std::cout << "Error: Audio buffer is full.\n";
  }
}

void AVData::load_video(){};

std::tuple<const uint8_t *, const uint8_t *> AVData::to_playback() {

  uint8_t *buffer_video = reinterpret_cast<uint8_t *>(m_data);
  uint8_t *buffer_audio = buffer_video + m_video_size;

  return {buffer_video, buffer_audio};
}

void AVData::reset_audio_buffer() {
  m_audio_index = 0;
}

AVData::~AVData() {
  delete[] m_audio_buffer;
  // delete[] m_data;
}
