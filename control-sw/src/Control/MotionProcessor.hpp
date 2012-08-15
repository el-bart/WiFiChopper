#ifndef INCLUDE_CONTROL_MOTIONPROCESSOR_HPP_FILE
#define INCLUDE_CONTROL_MOTIONPROCESSOR_HPP_FILE

#include "IO/ProtoRTB.hpp"
#include "Control/Input.hpp"

namespace Control
{

class MotionProcessor
{
public:
  explicit MotionProcessor(double rotFactor):
    rotFactor_( rotFactor )
  { }

  IO::ProtoRTB::EngineSpeed transform(Control::Input::Movement mov, double throttle) const;

private:
  double rotFactor_;
};

}

#endif
