#include <algorithm>
#include <cmath>
#include <cassert>

#include "IO/ProtoRTB.hpp"
#include "Util/hex.hpp"


namespace IO
{

double ProtoRTB::Accel::norm(void) const
{
  return sqrt(x_*x_ + y_*y_ + z_*z_);
}



ProtoRTB::EngineSpeed::EngineSpeed(const double main1, const double main2, const double rear):
  main1_(main1),
  main2_(main2),
  rear_(rear)
{
  if( main1_ < 0 || 1 < main1_ )
    throw Util::Exception( UTIL_LOCSTRM << "main1 engine speed out of range: " << main1_ );
  if( main2_ < 0 || 1 < main2_ )
    throw Util::Exception( UTIL_LOCSTRM << "main2 engine speed out of range: " << main2_ );
  if( rear_ < -1 || 1 < rear_ )
    throw Util::Exception( UTIL_LOCSTRM << "rear engine speed out of range: " << rear_ );
}



ProtoRTB::ProtoRTB(LineCommPtr lc, const double timeout):
  lc_( checkInPtr( std::move(lc) ) ),
  timeout_( timeout ),
  raw1G_( computeRaw1G() )
{
  assert( lc_.get()!=NULL );
}


std::string ProtoRTB::hello(void)
{
  lc_->send("hello");
  return lc_->read(timeout_);
}


ProtoRTB::Accel ProtoRTB::accelerometer(void)
{
  const Accel  raw = readRawAccelerometer();
  // normalize according to the 1G sample, and the current reading
  const double r1g = raw1G_.norm();
  return Accel{ raw.x_/r1g, raw.y_/r1g, raw.z_/r1g };   // final result
}


ProtoRTB::EngineSpeed ProtoRTB::engineSpeed(void)
{
  lc_->send("eng?");
  const std::string d = lc_->read(timeout_);
  if( d.length() != 3*2 +2 +1 || d[2] != ' ' || d[5] != ' ' )
    throw Util::Exception( UTIL_LOCSTRM << "invalid response for engine speed query: " << d );

  // parse values
  const uint8_t main1   = (Util::fromHex(d[0])<<4) | Util::fromHex(d[1]);
  const uint8_t main2   = (Util::fromHex(d[3])<<4) | Util::fromHex(d[4]);
  const uint8_t rear    = (Util::fromHex(d[7])<<4) | Util::fromHex(d[8]);
  // check sign
  double        sigRear;
  switch(d[6])
  {
    case '+': sigRear = +1; break;
    case '-': sigRear = -1; break;
    default:  throw Util::Exception( UTIL_LOCSTRM << "invalid sign of rear engine: " << d[6] );
  }

  // return final values
  return EngineSpeed{ main1/255.0, main2/255.0, sigRear*rear/255.0 };
}


void ProtoRTB::engineSpeed(EngineSpeed spd)
{
  // prepare message to be sent
  std::string d;

  // inset command name
  for(const char *it="engset "; *it!=0; ++it)
    d.push_back(*it);
  // add main1
  {
    const uint8_t num = uint8_t( spd.getMain1()*255 );
    const auto    hex = Util::toHex(num);
    d.push_back(hex.first);
    d.push_back(hex.second);
    d.push_back(' ');
  }
  // add main2
  {
    const uint8_t num = uint8_t( spd.getMain2()*255 );
    const auto    hex = Util::toHex(num);
    d.push_back(hex.first);
    d.push_back(hex.second);
    d.push_back(' ');
  }
  // add rear
  {
    const char    sig = spd.getRear()>=0 ? '+' : '-';
    const uint8_t num = uint8_t( fabs(spd.getRear())*255 );
    const auto    hex = Util::toHex(num);
    d.push_back(sig);
    d.push_back(hex.first);
    d.push_back(hex.second);
  }

  // send command
  lc_->send(d);
  // await response
  const std::string r = lc_->read(timeout_);
  if( r != std::string( d.begin()+7, d.end() ) )
    throw Util::Exception( UTIL_LOCSTRM << "invalid response: " << r );
}


double ProtoRTB::batteryVoltage(void)
{
  // send message
  lc_->send("vin?");
  // read response
  const std::string r = lc_->read(timeout_);
  // sanity check
  if( r.length() != 2 )
    throw Util::Exception( UTIL_LOCSTRM << "invalid voltage request response: " << r );
  // parse it
  const uint8_t h = (Util::fromHex(r[0])<<4) | Util::fromHex(r[1]);
  // transform it to the voltage value in Volts
  return (3*h*4*5)/1024.0;
}


ProtoRTB::Accel ProtoRTB::readRawAccelerometer(void)
{
  // read from the device
  lc_->send("accel?");
  const std::string r = lc_->read(timeout_);

  // sanity checks
  if( r.size() != 3*2 + 2 || r[2] != ' ' || r[5] != ' ' )
    throw Util::Exception( UTIL_LOCSTRM << "invalid response: " << r );

  // parse reposen
  const double ox = (Util::fromHex(r[0])<<4) | Util::fromHex(r[1]);
  const double oy = (Util::fromHex(r[3])<<4) | Util::fromHex(r[4]);
  const double oz = (Util::fromHex(r[6])<<4) | Util::fromHex(r[7]);

  return Accel{ox, oy, oz};
}


ProtoRTB::Accel ProtoRTB::computeRaw1G(void)
{
  constexpr size_t reads    = 16;
  double           axies[3] = {0,0,0};
  for(size_t i=0; i<reads; ++i)
  {
    const Accel r = readRawAccelerometer();
    axies[0] += r.x_;
    axies[1] += r.y_;
    axies[2] += r.z_;
  }

  return Accel{ axies[0]/reads, axies[1]/reads, axies[2]/reads };
}


LineCommPtr ProtoRTB::checkInPtr(LineCommPtr ptr) const
{
  if( !ptr )
    throw Util::Exception( UTIL_LOCSTRM << "LineComm pointer cannot be NULL" );
  return std::move(ptr);
}

}
