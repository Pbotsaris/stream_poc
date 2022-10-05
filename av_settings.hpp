#ifndef AV_SETTINGS_H
#define AV_SETTINGS_H

extern "C" {
 #include <libavcodec/avcodec.h>
}

/* audio */

class AudioSettings {

  enum CodecID {
    mp3 = AV_CODEC_ID_MP3,
    mp2 = AV_CODEC_ID_MP2,
    vorbis = AV_CODEC_ID_VORBIS,
    opus = AV_CODEC_ID_OPUS,
    flac = AV_CODEC_ID_FLAC,
    aac = AV_CODEC_ID_AAC,
    ac3 = AV_CODEC_ID_AC3
  };

public:
  static AudioSettings *get_instance();

  static void delete_instance();

  int samplerate() const;
  int bitrate() const;
  int channels() const;
  bool is_mono() const;
  int buffer_size() const;
  int buffer_size_in_samples() const;
  int bit_multiplier() const;
  int converter_max_tries() const;
  AVSampleFormat sample_format() const;
  AVCodecID codec_id() const;

  AudioSettings(AudioSettings &lhs) = delete;
  void operator=(AudioSettings &lhs) = delete;

private:
  static AudioSettings *m_instance;

  CodecID m_codec_id = mp3;
  int m_channels = 1;
  int m_bitrate = 64000;
  int m_samplerate = 44100;
  AVSampleFormat m_sample_format = AV_SAMPLE_FMT_S16P; // bit depth int 16 bits
  /* buffer of 1 video frame of uncompressed audio @ 44100 and 25fps  */
  int m_bit_multiplier = 2; // for 16 bits. need to change this if another format.
                            
  // IN SAMPLES
  int m_buffer_size = 3528; // 44100 / 25 * 2(1 sample = 2 bytes @ 16bits) = 3528
  int m_converter_max_tries = 10;

  AudioSettings();
};

/* Video */

class VideoSettings {

public:
  static VideoSettings *get_instance();

  static void delete_instance();

  VideoSettings(VideoSettings &lhs) = delete;
  void operator=(VideoSettings &lhs) = delete;

  int bitrate() const;
  int width() const;
  int height() const;
  int framerate() const;
  int capture_size_frames() const;

private:
  static VideoSettings *m_instance;

  int m_bitrate = 40000;
  int m_height = 480;
  int m_width = 640;
  int m_framerate = 25;
  int m_capture_size = 5; // video frames @ 25 fps

  VideoSettings();
};

#endif
