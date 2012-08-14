#ifndef INCLUDE_IO_PROTORTB_HPP_FILE
#define INCLUDE_IO_PROTORTB_HPP_FILE

#include <string>
#include <memory>

#include "Util/Exception.hpp"
#include "IO/TextLine.hpp"

namespace IO
{

class ProtoRTB
{
public:
  struct Accel
  {
    double norm(void) const;

    // norm() equal to 1 means 1G
    double x_;
    double y_;
    double z_;
  };

  class EngineSpeed
  {
  public:
    EngineSpeed(double main1, double main2, double rear);

    double getMain1(void) const { return main1_; }
    double getMain2(void) const { return main2_; }
    double getRear(void) const { return rear_; }

  private:
    double main1_;  // [0..+1]
    double main2_;  // [0..+1]
    double rear_;   // [-1..+1]
  };

  typedef std::unique_ptr<TextLine> TextLinePtr;

  ProtoRTB(TextLinePtr tl, double timeout);

  std::string hello(void);
  Accel accelerometer(void);
  EngineSpeed engineSpeed(void);
  void engineSpeed(EngineSpeed spd);
  double batteryVoltage(void);

private:
  Accel readRawAccelerometer(void);
  Accel computeRaw1G(void);

  TextLinePtr checkInPtr(TextLinePtr ptr) const;

  TextLinePtr  tl_;
  const double timeout_;
  const Accel  raw1G_;
};

}

#endif
