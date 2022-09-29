#include "av_settings.hpp"

/* Video Settings */

AudioSettings* AudioSettings::m_instance = nullptr;
VideoSettings* VideoSettings::m_instance = nullptr;

AudioSettings::AudioSettings(){};

int AudioSettings::bitrate() const { return m_bitrate; }
int AudioSettings::samplerate() const { return m_samplerate; }
int AudioSettings::channels() const { return m_channels; }
bool AudioSettings::is_mono() const { return m_channels == 1; }
int AudioSettings::buffer_size() const { return m_buffer_size; }
AVSampleFormat AudioSettings::sample_format() const { return m_sample_format; }
AVCodecID AudioSettings::codec_id() const {
  return static_cast<AVCodecID>(m_codec_id);
}

void AudioSettings::delete_instance() { delete m_instance; }

AudioSettings *AudioSettings::get_instance() {

  if (m_instance == nullptr) {
    m_instance = new AudioSettings();
  }

  return m_instance;
};

/* Video Settings */

VideoSettings::VideoSettings(){};

int VideoSettings::bitrate() const { return m_bitrate; }
int VideoSettings::width() const { return m_width; }
int VideoSettings::framerate() const { return m_framerate; }
int VideoSettings::height() const { return m_height; }
int VideoSettings::buffer_size_in_frames() const { return m_nb_frames_in_buffer; }
int VideoSettings::buffer_size() const { return m_buffer_size; }

void VideoSettings::delete_instance() { delete m_instance; };

VideoSettings* VideoSettings::get_instance() {

    if (m_instance == nullptr) {
      m_instance = new VideoSettings();
    }

    return m_instance;
  };

