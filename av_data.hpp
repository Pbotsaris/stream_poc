
#ifndef AV_DATA_H
#define AV_DATA_H

#include <iostream>
#include <tuple>

class AVData {

public:
  AVData();

  void alloc_video(std::size_t t_size);

  void load_audio(uint8_t *t_audio_data, std::size_t t_size);
  void load_video();

  bool empty() const;
  std::byte *get_data() const;
  std::size_t size() const;
  std::size_t audio_size() const;


  void reset_audio_buffer();

  ~AVData();

private:
  uint8_t*    m_audio_buffer = nullptr;
  std::size_t m_audio_size = 0;
  std::size_t m_max_audio_size; // in bytes

  uint8_t*    m_video_buffer = nullptr;
  std::size_t m_video_size = 0;
  std::size_t m_max_video_size; // in bytes
                            

  std::byte *m_data = nullptr;
  // const Data::type     m_type = Data::AudioVideo;
};

#endif
