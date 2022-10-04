#include "webcam.hpp"

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <string>
#include <sys/socket.h>
#include <vector>

#define QT 0

VideoSettings *Webcam::m_VIDEO_SETTINGS = VideoSettings::get_instance();

Webcam::Webcam() {

  m_camera = 0; // TODO change that to find available cameras instead

  int result = m_capture.open(m_camera);
  if (false == result) {
    std::cout << "Failed to open camera";
    m_valid = false;
  }

  m_capture.set(cv::CAP_PROP_FRAME_WIDTH, m_VIDEO_SETTINGS->width());
  m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, m_VIDEO_SETTINGS->height());
  m_frames = cv::Mat::zeros(m_VIDEO_SETTINGS->height(),
                            m_VIDEO_SETTINGS->width(), CV_8UC3);
}

Webcam::~Webcam() {
  m_capture.release();
  cv::destroyAllWindows();
}

void Webcam::capture() {

  int nb_frames = m_VIDEO_SETTINGS->capture_size_frames();
  int count = 0;

  //m_data_size = m_frames.total() * m_frames.channels();

  if (m_valid) {

    while (nb_frames >= count) {
      m_capture >> m_frames;
      std::vector<uchar> buffer;
      bool result = cv::imencode(".jpeg", m_frames, buffer);
      if (result) {

      } else {
        std::cout << "Error: Failed to encode frame";
        m_valid = false;
      }
    }
  }
}

void Webcam::wait() const { cv::waitKey(25); }

bool Webcam::valid() const { return m_valid; }

void Webcam::loop() {
  //  int key;
  //  enable();
  //  while (false == m_stop_stream) {
  //    capture();
  //    // show(m_frames);
  //
  //    // buffer to encode into
  //    std::vector<uchar> buffer;
  //
  //    // encoding to jpeg into buffer(has to be vector<uchar>)
  //    if (cv::imencode(".jpeg", m_frames, buffer) == false)
  //      std::cout << "encoding frame failed" << std::endl;
  //
  //    // weird recast to check size found on SO
  //    std::cout << "encoded buffer.data() -> "
  //              << std::strlen(reinterpret_cast<char *>(buffer.data()))
  //              << std::endl;
  //
  //    // decode back to m_frames Mat
  //    m_frames = cv::imdecode(buffer, cv::IMREAD_COLOR);
  //
  //    std::cout << "m_frames.data() decode -> "
  //              << std::strlen((char *)m_frames.data) << std::endl;
  //
  //    key = cv::waitKey(25);
  //    if ('w' == key) {
  //      disable();
  //    }
  //    if ('e' == key) {
  //      enable();
  //    }
  //    if ('q' == key) {
  //      return;
  //    }
  //  }
}

//#if QT
// cv::cvtColor(frame(), frame(), cv::COLOR_BGR2RGBA);
//
// qt_image = QImage((const unsigned char *)(frame().data), frame.cols,
// frame.rows,
//                  QImage::Format_RGB888);
// ui->label->setPixmap(QPixmap::fromImage(qt_image));
// ui->label->resize(ui->label.pixmap()->size());
//#endif

/*
send(other_socket, frames().data, size(), 0)
*/
