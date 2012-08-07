#include <tut/tut.hpp>
#include <thread>

#include "ServoCtrl/ReadThread.hpp"

using namespace ServoCtrl;

namespace
{
struct TestClass
{
  TestClass(void):
    dev_{"/dev/ttyUSB0"},
    rd_{ new SerialPort{dev_, SerialPort::READ} },
    wr_{ new SerialPort{dev_, SerialPort::WRITE} },
    th_{rd_}
  {
  }

  const char      *dev_;
  SerialPortPtrNN  rd_;
  SerialPortPtrNN  wr_;
  ReadThread       th_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ServoCtrl/ReadThread");
} // unnamed namespace


namespace tut
{

// test destruction of thread, when nothing has been done
template<>
template<>
void testObj::test<1>(void)
{
  // test d-tor
}

// test getting promise and reading it
template<>
template<>
void testObj::test<2>(void)
{
  wr_->writeLine("as88?");
  Response r=th_.enqueue( ServoName{'a'} );
  ensure("invalid response", r.ok() );
}

// test timeout when nothing arrives
template<>
template<>
void testObj::test<3>(void)
{
  Response r=th_.enqueue( ServoName{ServoName::first()} );
  ensure("error not risen", r.error() );
}

} // namespace tut
