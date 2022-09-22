#include "webcam.hpp"

#include <iostream>
#include <opencv2/highgui.hpp>

Webcam::Webcam()
{

}

Webcam::~Webcam()
{

}

void Webcam::release()
{
  m_capture.release();
  cv::destroyAllWindows();
}

void Webcam::create_window()
{
  cv::namedWindow("Camera");
  m_window = true;
}

bool Webcam::create_camera()
{
  m_camera = 0; // TODO change that to find available cameras instead

  int ret = m_capture.open(m_camera);
  if( false == ret )
  {
    std::cout << "Couldn't open camera";
    return false;
  }

  m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  m_frames = cv::Mat::zeros(480, 648, CV_8UC3);
  
  m_data_size = m_frames.total()*m_frames.channels();

  return true;
}

uchar* Webcam::data()
{
  m_capture >> m_frames;
  return m_frames.data;
}

int Webcam::data_size()
{
  return m_data_size;
}

void Webcam::loop()
{
  int key;

  while ( false == m_stop_stream)
  {
    data();
    
    cv::imshow("Camera", m_frames);

    key = cv::waitKey(25);
    if ( 'q' == key)
    {
      return;
    }
  }
}