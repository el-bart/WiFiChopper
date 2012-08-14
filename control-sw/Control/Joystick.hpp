#ifndef INCLUDE_CONTROL_JOYSTICK_HPP_FILE
#define INCLUDE_CONTROL_JOYSTICK_HPP_FILE

#include <string>
#include <memory>

#include "Control/Input.hpp"


namespace Control
{

class Joystick: public Input
{
public:
  // device info
  struct Info
  {
    unsigned driverVer_[3];
    char     name_[128];
    unsigned axis_;
    unsigned buttons_;
  };

  // map of the axis-to-number
  struct AxisMap
  {
    struct Axis
    {
      unsigned num_;
      bool     invert_;
    };

    // OX and OY
    bool useXY_;
    Axis ox_;
    Axis oy_;
    // throttle
    bool useTh_;
    Axis th_;
  };

  Joystick(const std::string& path, AxisMap axisMap);

  const Info& info(void) const { return info_; }

private:
  virtual void updateImpl(void);
  virtual const std::string& nameImpl(void);

  // device info
  const Info              info_;
  const std::string       name_;
  const AxisMap           axisMap_;
  // reads buffer
  std::unique_ptr<int[]>  axis_;
  std::unique_ptr<char[]> buttons_;
};

}

#endif
