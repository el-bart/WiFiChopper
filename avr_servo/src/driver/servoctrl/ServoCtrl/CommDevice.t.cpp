#include <tut/tut.hpp>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ServoCtrl/CommDevice.hpp"

using namespace ServoCtrl;

namespace
{
struct TestClass
{
  TestClass(void):
    cd_("/dev/ttyUSB0")
  {
  }

  CommDevice cd_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ServoCtrl/CommDevice");
} // unnamed namespace


namespace tut
{

// open whatever and try sending something
template<>
template<>
void testObj::test<1>(void)
{
  ensure("invalid response", cd_.run("as7f?\n").ok() );
}

// test error condition
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    cd_.run("wtf?\n");
    fail("no exception on error");
  }
  catch(const ServoName::ExceptionInvalidServo&)
  {
    // this is expected
  }
}

// try sending multiple commands
template<>
template<>
void testObj::test<3>(void)
{
  ensure("invalid response 1", cd_.run("as30?\n").ok() );
  ensure("invalid response 2", cd_.run("asa0?\n").ok() );
}

// try sending multiple commands: error, ok - it should recover
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    cd_.run("wtf?\n");
    fail("no exception on error message");
  }
  catch(const ServoName::ExceptionInvalidServo&)
  {
    // this is expected
  }
  ensure("invalid response", cd_.run("asa0?\n").ok() );
}

// test sending fast commands
template<>
template<>
void testObj::test<5>(void)
{
  for(char c: {'3','5','7','9','b','d'})
  {
    char buf[]="asX0?\n";
    buf[2]=c;
    cd_.run(buf);
  }
}

// test something to far servo
// this reproduces bug when controller crashed on this
template<>
template<>
void testObj::test<6>(void)
{
  cd_.run("os70?\n");
}

// check proper recover, after sending non-full command followed by the correct one
// this is a bug test, for the older implementation of command queue.
template<>
template<>
void testObj::test<7>(void)
{
  cd_.run("os0?\n");        // this command is invalid
  cd_.run("os70?\n");       // this one is fine
}

// check proper recover, after sending invalid command followed by the correct one
template<>
template<>
void testObj::test<8>(void)
{
  cd_.run("osXx?\n");       // this command is invalid
  cd_.run("os70?\n");       // this one is fine
}

// check if extreamly long packets are parsed as one, invalid data set
// this is a bug test, for the older implementation of command queue on uC.
template<>
template<>
void testObj::test<9>(void)
{
  cd_.run("ovvXXyyZZwwQQssRRttBlah...\n");      // this command is invalid
  cd_.run("os70?\n");                           // this one is fine
}

// check if longer sequence of CR/CL chars is end-of-packet
template<>
template<>
void testObj::test<10>(void)
{
  cd_.run("os70?\r\n\r");
  cd_.run("bs70?\n");
}

} // namespace tut
