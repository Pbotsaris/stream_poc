#ifndef WEBCAM_HPP
#define WEBCAM_HPP

#include <opencv2/opencv.hpp>

class Webcam
{
public:

  Webcam();
  ~Webcam();
  void                            create_window();
  uchar*                          data();
  bool                            create_camera();
  int                             data_size();
  void                            loop();
  void                            release();

private:

  int                              m_camera;
  cv::Mat                          m_frames;
  cv::VideoCapture                 m_capture;
  bool                             m_window;
  bool                             m_stop_stream = false;
  int                              m_data_size;
  
};

#endif // WEBCAM_HPP