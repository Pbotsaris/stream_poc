#ifndef WEBCAM_HPP
#define WEBCAM_HPP

#include <opencv2/opencv.hpp>
#include "av_settings.hpp"

class Webcam
{
public:

  Webcam();
  ~Webcam();
  void                            capture();
  void                            loop();
  void                            wait() const;
  bool                            valid() const;

private:
  int                              m_camera;
  cv::Mat                          m_frames;
  cv::VideoCapture                 m_capture;
  bool                             m_valid = true;

  static VideoSettings *m_VIDEO_SETTINGS;
  
};

#endif // WEBCAM_HPP
