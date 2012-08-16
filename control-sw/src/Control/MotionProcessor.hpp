#ifndef INCLUDE_CONTROL_MOTIONPROCESSOR_HPP_FILE
#define INCLUDE_CONTROL_MOTIONPROCESSOR_HPP_FILE

#include "IO/ProtoRTB.hpp"
#include "Control/Input.hpp"

namespace Control
{

class MotionProcessor
{
public:
  MotionProcessor(double rotFactor, double trim):
    rotFactor_( rotFactor ),
    trim_( trim )
  { }

  IO::ProtoRTB::EngineSpeed transform(Control::Input::Movement mov, double throttle) const;

private:
  const double rotFactor_;
  const double trim_;
};

}

#endif
