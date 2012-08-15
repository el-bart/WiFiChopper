#include "Control/MotionProcessor.hpp"
#include "Util/keepInRange.hpp"


namespace Control
{

IO::ProtoRTB::EngineSpeed MotionProcessor::transform(Control::Input::Movement mov, double throttle) const
{
  const double diff  = mov.y_ * rotFactor_;
  const double main1 = Util::keepInRange(throttle + diff, 0.0, 1.0);
  const double main2 = Util::keepInRange(throttle - diff, 0.0, 1.0);
  const double rear  = mov.x_;
  return IO::ProtoRTB::EngineSpeed(main1, main2, rear);
}

}
