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
  struct Info
  {
    unsigned driverVer_[3];
    char     name_[128];
    unsigned axis_;
    unsigned buttons_;
  };

  explicit Joystick(const std::string& path);

private:
  virtual void updateImpl(void);
  virtual const std::string& nameImpl(void);

  // device info
  const Info              info_;
  const std::string       name_;
  // reads buffer
  std::unique_ptr<int[]>  axis_;
  std::unique_ptr<char[]> buttons_;
};

}

#endif
