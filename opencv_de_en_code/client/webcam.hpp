#ifndef WEBCAM_HPP
#define WEBCAM_HPP

#include <opencv2/opencv.hpp>

class Webcam
{
public:

  Webcam();
  ~Webcam();
  void                            create_window();
  void                            capture();
  bool                            create_camera();
  int                             data_size();
  void                            loop();
  void                            release();
  void                            show(cv::Mat &t_frames);
  cv::Mat&                        frames();
  void                            enable();                            
  void                            disable();                            
  bool                            enabled();

  bool                             m_stop_stream = false;
  cv::Mat                          m_frames;                            

private:

  int                              m_camera;
  // cv::Mat                          m_frames;
  cv::VideoCapture                 m_capture;
  bool                             m_window;
  // bool                             m_stop_stream = false;
  int                              m_data_size;
  bool                             m_webcam_enabled = false;
  
};

#endif // WEBCAM_HPP