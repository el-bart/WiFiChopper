#ifndef INCLUDE_CONTROL_INPUT_HPP_FILE
#define INCLUDE_CONTROL_INPUT_HPP_FILE

#include <string>
#include <memory>

#include "Util/UniqueDescriptor.hpp"
#include "Util/Exception.hpp"


namespace Control
{

class Input
{
public:
  struct Movement
  {
    double x_;
    double y_;
  };

  struct DeviceOpenFailed: public Util::Exception
  {
    explicit DeviceOpenFailed(const std::string& path);
  };


  // noncopyable:
  Input(const Input&) = delete;
  Input& operator=(const Input&) = delete;
  // nonmovable:
  Input(Input&&) = delete;
  Input& operator=(Input&&) = delete;

  virtual ~Input(void) { }

  Movement getMovement(void) const { return movement_; }
  double getThrottle(void) const { return throttle_; }

  int rawDescriptor(void) const { return fd_.get(); }

  void update(void) { updateImpl(); }
  const std::string& name(void) { return nameImpl(); }
  const std::string& path(void) { return path_; }

protected:
  Input(Util::UniqueDescriptor fd, std::string path);

  void setMovement(const Movement m) { movement_ = m; }
  void setThrottle(const double t) { throttle_ = t; }

private:
  // updates readings from the input source
  virtual void updateImpl(void) = 0;
  // extra info
  virtual const std::string& nameImpl(void) = 0;

  // device
  Util::UniqueDescriptor fd_;
  std::string            path_;
  // current state
  Movement               movement_;
  double                 throttle_;
};


typedef std::unique_ptr<Input> InputPtr;

}

#endif
