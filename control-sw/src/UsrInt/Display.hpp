#ifndef INCLUDE_USRINT_DISPLAY_HPP_FILE
#define INCLUDE_USRINT_DISPLAY_HPP_FILE

#include "Util/Exception.hpp"
#include "IO/ProtoRTB.hpp"
#include "UsrInt/FrameGrabber.hpp"
#include "UsrInt/VideoWriter.hpp"
#include "UsrInt/TextAdder.hpp"


namespace UsrInt
{

class Display
{
public:
  struct Info
  {
    double              batteryVoltage_;
    IO::ProtoRTB::Accel acceleration_;
  };

  explicit Display(FrameGrabberPtr fg, bool flipH=false, bool flipV=false);

  void update(const Info& info);

  const std::string& outputFile(void) const { return vw_.fileName(); }

private:
  FrameGrabberPtr fg_;
  VideoWriter     vw_;
  TextAdder       txt_;
  bool            flip_;
  int             flipDir_;
};

}

#endif
