#ifndef INCLUDE_USRINT_FRAMEGRABBER_HPP_FILE
#define INCLUDE_USRINT_FRAMEGRABBER_HPP_FILE

#include <memory>
#include <opencv2/opencv.hpp>

#include "Util/Exception.hpp"


namespace UsrInt
{

class FrameGrabber
{
public:
  FrameGrabber(void) { }
  virtual ~FrameGrabber(void) { }

  // noncopyable
  FrameGrabber(const FrameGrabber&) = delete;
  FrameGrabber& operator=(const FrameGrabber&) = delete;

  cv::Mat  grab(const double timeout = 0.250) { return grabImpl(timeout); }
  cv::Size size(void) { return sizeImpl(); }

private:
  virtual cv::Mat  grabImpl(double timeout) = 0;
  virtual cv::Size sizeImpl(void) = 0;
};


typedef std::unique_ptr<FrameGrabber> FrameGrabberPtr;

}

#endif
