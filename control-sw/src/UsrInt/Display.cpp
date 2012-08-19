#include <sstream>
#include <iomanip>
#include <algorithm>

#include "UsrInt/Display.hpp"


namespace UsrInt
{

namespace
{
int makeFlipDir(bool fh, bool fv)
{
  if(fh && fv)  // H + V
    return -1;
  if(fh)        // H only
    return 0;
  if(fv)        // V only
    return 1;
  return 0x42;  // doesn't matter ;)
}
} // unnamed namespace


Display::Display(FrameGrabberPtr fg, bool flipH, bool flipV):
  fg_( std::move(fg) ),
  txt_( 250, 30, CV_RGB(0,255,0), 1.3 ),
  flip_( flipH || flipV ),
  flipDir_( makeFlipDir(flipH, flipV) )
{ }


void Display::update(const Info& info)
{
  // get next frame
  cv::Mat in = fg_->grab(0.100);        // 0.1[s] is max we can wait
  cv::Mat frame;                        // rotated frame
  // perform rotation, if needed
  if(flip_)
    cv::flip(in, frame, flipDir_);
  else
    frame = in;

  // add battery info
  {
    std::stringstream ss;
    ss << std::setprecision(2) << std::fixed;
    ss << "Batt.:  " << info.batteryVoltage_ << "V";
    txt_.addLine( frame, 0, ss.str() );
  }

  // add accelereation vector
  {
    std::stringstream ss;
    ss << std::setprecision(2) << std::fixed;
    ss << "Accel.: " << info.acceleration_.x_ << " "
                     << info.acceleration_.y_ << " "
                     << info.acceleration_.z_;
    txt_.addLine( frame, 1, ss.str() );
  }

  // add acceleration value
  {
    std::stringstream ss;
    ss << std::setprecision(2) << std::fixed;
    ss << "Accel.: " << info.acceleration_.norm() << "G";
    txt_.addLine( frame, 2, ss.str() );
  }

  // display new frame
  imshow( "WiFiChopper", frame );
}


}
