#ifndef INCLUDE_USRINT_FRAMEGRABBER_HPP_FILE
#define INCLUDE_USRINT_FRAMEGRABBER_HPP_FILE

#include <chrono>
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

  void setExposureTime(const std::chrono::microseconds us) { setExposureTimeImpl(us); }
  void setAutoExposureTime(void) { setAutoExposureTimeImpl(); }
  void autoWhiteBalance(const bool set) { autoWhiteBalanceImpl(set); }

private:
  virtual cv::Mat  grabImpl(double timeout) = 0;
  virtual cv::Size sizeImpl(void) = 0;
  virtual void setExposureTimeImpl(const std::chrono::microseconds us) = 0;
  virtual void setAutoExposureTimeImpl(void) = 0;
  virtual void autoWhiteBalanceImpl(const bool set) = 0;
};


typedef std::unique_ptr<FrameGrabber> FrameGrabberPtr;

}

#endif
