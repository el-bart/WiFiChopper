#ifndef INCLUDE_USRINT_FRAMEGRABBERV4L_HPP_FILE
#define INCLUDE_USRINT_FRAMEGRABBERV4L_HPP_FILE

#include <vector>
#include <libv4l2.h>

#include "Util/UniqueDescriptor.hpp"
#include "Util/MappedMem.hpp"
#include "UsrInt/FrameGrabber.hpp"

namespace UsrInt
{

class FrameGrabberV4L: public FrameGrabber
{
public:
  FrameGrabberV4L(std::string devPath, size_t width, size_t height);

private:
  typedef Util::MappedMem<void, v4l2_mmap, v4l2_munmap> MMem;

  virtual cv::Mat  grabImpl(double timeout);
  virtual cv::Size sizeImpl(void);
  virtual void setExposureTimeImpl(const std::chrono::microseconds us);
  virtual void setAutoExposureTimeImpl(void);
  virtual void autoWhiteBalanceImpl(const bool set);

  Util::UniqueDescriptor openDevice(void) const;
  void init(size_t width, size_t height);
  void startCapture(void);
  cv::Mat toRGB(void* mem, size_t length) const;

  std::string            devPath_;
  Util::UniqueDescriptor dev_;

  unsigned width_;
  unsigned height_;
  unsigned bytesPerLine_;

  std::vector<MMem> buffers_;
};

}

#endif
