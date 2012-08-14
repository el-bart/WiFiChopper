#include <tut/tut.hpp>
#include <cmath>

#include "IO/ProtoRTB.hpp"

using namespace std;
using namespace IO;

namespace
{

struct LineCommMock: public LineComm
{
  LineCommMock(void):
    toSend_( "10 00 00" ),
    raw1G_{0x10, 0x00, 0x00}
  { }

  virtual void send(const std::string& line)
  {
    received_ = line;
  }

  virtual std::string read(const double timeout)
  {
    return toSend_;
  }

  string                toSend_;
  string                received_;
  const ProtoRTB::Accel raw1G_;
};


struct TestClass
{
  TestClass(void):
    lcm_( new LineCommMock ),
    prot_( LineCommPtr{lcm_}, 2.0 )
  {
    // erase settings required for initial setup for 1G raw vector
    lcm_->toSend_   = "NOT SET";
    lcm_->received_ = "NOTHING RECEIVED YET";
  }

  void checkDouble(const double value, const double expected, const char *msg = "invalid value", const double eps=0.001) const
  {
    if( fabs(value-expected) > eps )
      tut::ensure_equals(msg, value, expected);
  }

  LineCommMock* lcm_;
  ProtoRTB      prot_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("IO/ProtoRTB");
} // unnamed namespace


namespace tut
{

// try sening hello and check the answer
template<>
template<>
void testObj::test<1>(void)
{
  lcm_->toSend_="hello response";
  const string r = prot_.hello();
  ensure_equals("invalid hello response", r, "hello response");
  ensure_equals("invalid command sent", lcm_->received_, "hello");
}

// try reading accelerometer
template<>
template<>
void testObj::test<2>(void)
{
  lcm_->toSend_="6F F2 A9";
  const ProtoRTB::Accel a = prot_.accelerometer();
  const double          n = lcm_->raw1G_.norm();
  checkDouble(a.x_, 0x6F/n, "invalid accelerometer reading or OX");
  checkDouble(a.y_, 0xF2/n, "invalid accelerometer reading or OY");
  checkDouble(a.z_, 0xA9/n, "invalid accelerometer reading or OZ");
  ensure_equals("invalid command sent", lcm_->received_, "accel?");
}

// try setting engine speed
template<>
template<>
void testObj::test<4>(void)
{
  lcm_->toSend_="41 52 -63";
  const ProtoRTB::EngineSpeed es(0x41/255.0, 0x52/255.0, -0x63/255.0);
  prot_.engineSpeed(es);
  ensure_equals("invalid request sent", lcm_->received_, "engset 41 52 -63");
}

// try reading engine speed
template<>
template<>
void testObj::test<5>(void)
{
  lcm_->toSend_="c1 2f +3a";
  const ProtoRTB::EngineSpeed es = prot_.engineSpeed();
  checkDouble( es.getMain1(), 0xC1/255.0, "invalid speed of main1" );
  checkDouble( es.getMain2(), 0x2F/255.0, "invalid speed of main2" );
  checkDouble( es.getRear(),  0x3A/255.0, "invalid speed of rear" );
  ensure_equals("invalid command sent", lcm_->received_, "eng?");
}

// try error while reading engine speed
template<>
template<>
void testObj::test<6>(void)
{
  lcm_->toSend_="ERROR: oops...";
  try
  {
    prot_.engineSpeed();
    fail("no exception when reading failed");
  }
  catch(const Util::Exception&)
  { }
}

// test getting battery voltage
template<>
template<>
void testObj::test<7>(void)
{
  lcm_->toSend_="A6";
  const double r = prot_.batteryVoltage();
  checkDouble( r, (3*0xA6*4*5)/1024.0, "invalid voltage" );
  ensure_equals("invalid command sent", lcm_->received_, "vin?");
}

// test initialization with nullptr
template<>
template<>
void testObj::test<8>(void)
{
  try
  {
    ProtoRTB prot( LineCommPtr{}, 1 );
    fail("no excaption on NULL API pointer");
  }
  catch(const Util::Exception&)
  { }
}

// test seting invalid main1 speed
template<>
template<>
void testObj::test<9>(void)
{
  // too small
  try
  {
    ProtoRTB::EngineSpeed spd(-0.1, 0.5, 0.5);
    fail("no error on negative main speed 1");
  }
  catch(const Util::Exception&)
  { }
  // too big
  try
  {
    ProtoRTB::EngineSpeed spd(1.1, 0.5, 0.5);
    fail("no error on too bigmain speed 1");
  }
  catch(const Util::Exception&)
  { }
}

// test setting invalid main2 speed
template<>
template<>
void testObj::test<10>(void)
{
  // too small
  try
  {
    ProtoRTB::EngineSpeed spd(0.5, -0.1, 0.5);
    fail("no error on negative main speed 2");
  }
  catch(const Util::Exception&)
  { }
  // too big
  try
  {
    ProtoRTB::EngineSpeed spd(0.5, 1.1, 0.5);
    fail("no error on too bigmain speed 2");
  }
  catch(const Util::Exception&)
  { }
}

// test setting invalid rear speed
template<>
template<>
void testObj::test<11>(void)
{
  // too small
  try
  {
    ProtoRTB::EngineSpeed spd(0.5, 0.5, -1.1);
    fail("no error on too small rear speed");
  }
  catch(const Util::Exception&)
  { }
  // too big
  try
  {
    ProtoRTB::EngineSpeed spd(0.5, 0.5, 1.1);
    fail("no error on too high rear speed");
  }
  catch(const Util::Exception&)
  { }
}

// test getting acceleromenter norm
template<>
template<>
void testObj::test<12>(void)
{
  {
    const ProtoRTB::Accel a{1,0,0};
    checkDouble( a.norm(), 1 );
  }
  {
    const ProtoRTB::Accel a{1,2,3};
    checkDouble( a.norm(), 3.7417 );
  }
}

// test invalid 1G measurement
template<>
template<>
void testObj::test<13>(void)
{
  try
  {
    ProtoRTB prot( LineCommPtr{}, 1 );
    fail("no excaption on NULL API pointer");
  }
  catch(const Util::Exception&)
  { }
}

// try detection of error in setting engine speed
template<>
template<>
void testObj::test<14>(void)
{
  //      here:     vv
  lcm_->toSend_="41 FF -63";
  const ProtoRTB::EngineSpeed es(0x41/255.0, 0x52/255.0, -0x63/255.0);
  try
  {
    prot_.engineSpeed(es);
    fail("no error when setting failed");
  }
  catch(const Util::Exception&)
  { }
}

} // namespace tut
