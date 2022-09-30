
#ifndef AV_DATA_H
#define AV_DATA_H

#include <iostream>
#include <tuple>

class AVData {

public:
  AVData();

  void load_audio(uint8_t *t_audio_data, std::size_t t_size);
  void load_video();

  std::tuple<const uint8_t *, const uint8_t *>
  to_playback(); // const to prevent pointer from being modified.

  bool empty() const;
  std::byte *get_data() const;
  std::size_t size() const;
  void reset_audio_buffer();



  ~AVData();

private:
  uint8_t*    m_audio_buffer = nullptr;
  std::size_t m_audio_index = 0;
  std::size_t m_video_size; // in bytes

  uint8_t*    m_video_buffer = nullptr;
  std::size_t m_video_index = 0;
  std::size_t m_audio_size; // in bytes
                            

  std::byte *m_data = nullptr;
  std::size_t m_size = 0;
  // const Data::type     m_type = Data::AudioVideo;
};

#endif
