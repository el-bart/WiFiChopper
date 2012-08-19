#include <sstream>
#include <iomanip>
#include <algorithm>

#include "UsrInt/Display.hpp"


namespace UsrInt
{

Display::Display(FrameGrabberPtr fg):
  fg_( std::move(fg) ),
  txt_( 250, 30, CV_RGB(0,255,0), 1.3 )
{ }


void Display::update(const Info& info)
{
  // get next frame
  cv::Mat frame = fg_->grab(0.100);     // 0.1[s] is max we can wait

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
