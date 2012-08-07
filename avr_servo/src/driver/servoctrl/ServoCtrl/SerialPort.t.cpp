#include <tut/tut.hpp>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ServoCtrl/SerialPort.hpp"

using namespace ServoCtrl;

namespace
{
struct TestClass
{
  TestClass(void):
    devPath_{"/dev/ttyUSB0"},
    rd_{ new SerialPort{devPath_, SerialPort::READ} },
    wr_{ new SerialPort{devPath_, SerialPort::WRITE} }
  {
  }

  const char                  *devPath_;
  std::unique_ptr<SerialPort>  rd_;
  std::unique_ptr<SerialPort>  wr_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ServoCtrl/SerialPort");
} // unnamed namespace


namespace tut
{

// try sending some random data, sending with new line
template<>
template<>
void testObj::test<1>(void)
{
  wr_->writeLine("with new line\n");
}

// test sending some data NOT ending with new line
template<>
template<>
void testObj::test<2>(void)
{
  wr_->writeLine("no new line");
}

// test error when reading with write part
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    wr_->readLine(100);
    fail("no exception on reading from output");
  }
  catch(const SerialPort::ExceptionTimeout&)
  {
    // this is expected
  }
}

// test error when writing with read part
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    rd_->writeLine("should throw");
    fail("no exception on writing to input");
  }
  catch(const SerialPort::ExceptionIO&)
  {
    // this is expected
  }
}

// test error when opening non-existing file
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    SerialPort cd("/non/existing/element", SerialPort::READ);
    fail("no exception on error");
  }
  catch(const SerialPort::ExceptionDevice&)
  {
    // this is expected
  }
}

// try sending and reading response
template<>
template<>
void testObj::test<6>(void)
{
  wr_->writeLine("as30?\n");
  ensure_equals("invalid response", rd_->readLine(100), "a-ok\n");
}

// try sending multiple commands
template<>
template<>
void testObj::test<7>(void)
{
  wr_->writeLine("as30?\n");
  wr_->writeLine("bs30?\n");
  ensure_equals("invalid response", rd_->readLine(100), "a-ok\n");
  ensure_equals("invalid response", rd_->readLine(100), "b-ok\n");
}

// check if CR is end-of-packet
template<>
template<>
void testObj::test<8>(void)
{
  wr_->writeLine("as40?\r");
  ensure_equals("invalid response", rd_->readLine(100), "a-ok\n");
}

// check if CL is end-of-packet
template<>
template<>
void testObj::test<9>(void)
{
  wr_->writeLine("as60?\n");
  ensure_equals("invalid response", rd_->readLine(100), "a-ok\n");
}

// test timeout on reading
template<>
template<>
void testObj::test<10>(void)
{
  try
  {
    rd_->readLine(1);
    fail("timeout not occurred");
  }
  catch(const SerialPort::ExceptionTimeout&)
  {
    // this is expected
  }
}

// test opening single instance in R/W mode
template<>
template<>
void testObj::test<11>(void)
{
  // destroy previous instances
  rd_.reset();
  wr_.reset();
  // do the test
  SerialPortPtrNN p{ new SerialPort{devPath_, SerialPort::READ|SerialPort::WRITE} };
  p->writeLine("as90?");
  ensure_equals("invalid response", p->readLine(100), "a-ok\n");
}

} // namespace tut
