#ifndef INCLUDE_USRINT_FRAMEGRABBERV4L_HPP_FILE
#define INCLUDE_USRINT_FRAMEGRABBERV4L_HPP_FILE

#include <vector>
#include <memory>
#include <cinttypes>

#include "Util/UniqueDescriptor.hpp"
#include "Util/MappedMem.hpp"
#include "UsrInt/FrameGrabber.hpp"

namespace UsrInt
{

class FrameGrabberV4L: public FrameGrabber
{
public:
  FrameGrabberV4L(std::string devPath);

private:
  typedef std::unique_ptr<uint8_t[]> ImageBuffer;
  typedef Util::MappedMem<void> MMem;

  virtual cv::Mat grabImpl(void);

  Util::UniqueDescriptor openDevice(void) const;
  void init(void);
  cv::Mat toRGB(ImageBuffer img) const;

  // device info
  std::string            devPath_;
  Util::UniqueDescriptor dev_;
  // image size data
  unsigned width_;
  unsigned height_;
  unsigned bytesPerLine_;
  unsigned imageBufferSize_;
};

}

#endif
