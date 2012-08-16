#include "Control/MotionProcessor.hpp"
#include "Util/keepInRange.hpp"


namespace Control
{

IO::ProtoRTB::EngineSpeed MotionProcessor::transform(Control::Input::Movement mov, double throttle) const
{
  // for sake of engines ;)
  if( throttle < 0.1 )
    return IO::ProtoRTB::EngineSpeed(0, 0, 0);

  // calculations
  const double diff  = mov.y_ * rotFactor_;
  const double main1 = Util::keepInRange(throttle + trim_ + diff, 0.0, 1.0);
  const double main2 = Util::keepInRange(throttle - trim_ - diff, 0.0, 1.0);
  const double rear  = mov.x_;

  // return computed values
  return IO::ProtoRTB::EngineSpeed(main1, main2, rear);
}

}
