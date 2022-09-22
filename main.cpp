#include "SDL_audio.h"
#include "audio_config.hpp"
#include "udp_conn.hpp"
#include "webcam.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_types.h>

#include <libavutil/frame.h>
#include <libavutil/mem.h>
#include <libavcodec/avcodec.h>

#define INPUT_AUDIO_DEVICES 1
#define OUTPUT_AUDIO_DEVICES 0

#define STREAM_AUDIO 0
#define STREAM_WEBCAM 1
#define FFMPEG 0

class AudioDevice {
public:
  enum Status { Invalid, Opened };

  AudioDevice(AudioConfig *config) : m_config(config) {
    SDL_AudioSpec want, have;

    SDL_zero(want);
    want.freq = 44100; 
    want.format = AUDIO_S16; // signed 16-bit samples in little-endian byte order  16bit, 32 float,  
    want.channels = 1;
    want.samples = 4096;
    want.callback = audio_recording_callback;

    m_dev =
        SDL_OpenAudioDevice(m_config->get_input().c_str(), INPUT_AUDIO_DEVICES,
                            &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    if (m_dev == 0) {
      SDL_Log("Failed to open audio: %s", SDL_GetError());
      m_status = Invalid;
    };

    m_byte_per_sample = have.channels * (SDL_AUDIO_BITSIZE(have.format) / 8);
    m_byte_per_second = have.freq * m_byte_per_sample;

    SDL_PauseAudioDevice(m_dev, 0);
    SDL_Delay(1000);
    SDL_PauseAudioDevice(m_dev, 1);

    m_status = Opened;
  };

private:
  SDL_AudioDeviceID m_dev;

  inline static UDPConn m_conn = UDPConn(6000, std::string("127.0.0.1"));

  Status m_status;
  AudioConfig *m_config;
  int m_byte_per_sample;
  int m_byte_per_second;

  static void audio_recording_callback(void *user_data, Uint8 *stream, int len) {
    m_conn.send_out(std::string("127.0.0.1"), std::string("65393"), stream, len);
  };
};

int main(void) {

#if STREAM_AUDIO
  
  /*
    * NOTE @Pedro Please take a look a few lines down where i defined an if that checks if ffmpeg is enabled there is
    *
    * SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)
    * A comment that I read somewhere that "I think ffmpeg should be used to play audio and SDL for video in this case."
    * But since we already have audio done by SDL we can skip FFMPEG and with the size of OpenCV buffer we may not need to compress
    * the audio and video
    * Also OpenCV has encoding and decoding wrapped around FFMPEG
    * Please read this, specifically "imencode" and "imdecode"
    * https://docs.opencv.org/3.4/d4/da8/group__imgcodecs.html
    * Please let's discuss this further.
    *
  */

  SDL_Init(SDL_INIT_AUDIO);
  const AVCodec *codec; // test of ffmpeg is linking

   // http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html

  AudioConfig *config = AudioConfig::get_instance();

  AudioConfig::DeviceListConst devs = config->list_input_name();

  auto dv = AudioDevice(config);

  AudioConfig::delete_instance();
#endif

#if STREAM_WEBCAM 

  Webcam camera;
  camera.create_window();
  camera.create_camera();
  camera.loop();
  camera.release();

#endif

#if FFMPEG
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) 
    {
        fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }

    // Init ffmpeg and setup some SDL stuff related to Audio
    setup_ffmpeg(argv[1]);

    VideoCapture cap(argv[1]); // open the default camera
    if (!cap.isOpened())  // check if we succeeded
    {
        std::cout << "Failed to load file!" << std::endl;
            return -1;
    }

    AVPacket packet;
    while (av_read_frame(pFormatCtx, &packet) >= 0) 
    {
        if (packet.stream_index == videoStream) 
        {
              // Actually this is were SYNC between audio/video would happen.
              // Right now I assume that every VIDEO packet contains an entire video frame, and that's not true. A video frame can be made by multiple packets!
              // But for the time being, assume 1 video frame == 1 video packet,
              // so instead of reading the frame through ffmpeg, I read it through OpenCV. 

              Mat frame;
              cap >> frame; // get a new frame from camera

          // do some processing on the frame, either as a Mat or as IplImage.
              // For educational purposes, applying a lame grayscale conversion
          IplImage ipl_frame = frame;
          for (int i = 0; i < ipl_frame.width * ipl_frame.height * ipl_frame.nChannels; i += ipl_frame.nChannels)
          {
                ipl_frame.imageData[i] = (ipl_frame.imageData[i] + ipl_frame.imageData[i+1] + ipl_frame.imageData[i+2])/3;   //B
                ipl_frame.imageData[i+1] = (ipl_frame.imageData[i] + ipl_frame.imageData[i+1] + ipl_frame.imageData[i+2])/3; //G
                ipl_frame.imageData[i+2] = (ipl_frame.imageData[i] + ipl_frame.imageData[i+1] + ipl_frame.imageData[i+2])/3; //R
            }

            // Display it on SDL window
            show_frame(&ipl_frame); 

            av_free_packet(&packet);
        }   
        else if (packet.stream_index == audioStream) 
        {
            packet_queue_put(&audioq, &packet);
        } 
        else 
        {
            av_free_packet(&packet);
        }

        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) 
        {
            case SDL_QUIT:
                SDL_FreeSurface(surface);
                SDL_Quit();
                break;

            default:
                break;  
        }
    }

    // the camera will be deinitialized automatically in VideoCapture destructor

    // Close the codec
    avcodec_close(pCodecCtx);

    // Close the video file
    av_close_input_file(pFormatCtx);

    return 0;
#endif

  return 0;
}
