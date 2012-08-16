#ifndef INCLUDE_USRINT_FRAMEGRABBERV4L_HPP_FILE
#define INCLUDE_USRINT_FRAMEGRABBERV4L_HPP_FILE

#include <vector>

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
  typedef Util::MappedMem<void> MMem;

  virtual cv::Mat grabImpl(void);

  Util::UniqueDescriptor openDevice(void) const;
  void init(void);
  void startCapture(void);
  cv::Mat toRGB(MMem& mm) const;

  std::string            devPath_;
  Util::UniqueDescriptor dev_;

  unsigned width_;
  unsigned height_;

  std::vector<MMem> buffers_;
};

}

#endif
