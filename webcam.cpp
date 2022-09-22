#include "webcam.hpp"

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <sys/socket.h>

#define QT 0

Webcam::Webcam()
{}

Webcam::~Webcam()
{}

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
  m_frames = cv::Mat::zeros(480, 640, CV_8UC3);
  
  m_data_size = m_frames.total()*m_frames.channels();

  return true;
}

void Webcam::capture()
{
  if ( true == m_webcam_enabled)
  {
    m_capture >> m_frames;
  }
  else 
  {
    m_frames = 0;
  }
}

int Webcam::data_size()
{
  return m_frames.total()*m_frames.channels();
}

cv::Mat& Webcam::frames()
{

  return m_frames;
}

void Webcam::show(cv::Mat &t_frames)
{
  cv::imshow("Camera", t_frames);
}

void Webcam::disable()
{
  m_webcam_enabled = false;
}

void Webcam::enable()
{
  m_webcam_enabled = true;
}

bool Webcam::enabled()
{
  return m_webcam_enabled;
}

void Webcam::loop()
{
  int key;
  enable();
  while ( false == m_stop_stream)
  {
    capture();
    show(m_frames);
    
    // Demo to check frame size when the camera is off
    std::cout << std::strlen((char*)m_frames.data) << std::endl;

    key = cv::waitKey(25);
    if ( 'w' == key)
    {
      disable();
    }
    if ( 'e' == key)
    {
      enable();
    }
    if( 'q' == key )
    {
      return;
    }
  }
}

#if QT
  cv::cvtColor(frame(), frame(), cv::COLOR_BGR2RGBA);
  qt_image = QImage((const unsigned char*)(frame().data), frame.cols, frame.rows, QImage::Format_RGB888);
  ui->label->setPixmap(QPixmap::fromImage(qt_image));
  ui->label->resize(ui->label.pixmap()->size());
#endif

/*


send(other_socket, frames().data, size(), 0)


*/