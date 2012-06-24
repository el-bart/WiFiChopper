#include <tut/tut.hpp>
#include <thread>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ServoCtrl/PromisesQueue.hpp"

using namespace ServoCtrl;

namespace
{
struct TestClass
{
  PromisesQueue q_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ServoCtrl/PromisesQueue");
} // unnamed namespace


namespace tut
{

// test destruction of non-empty queue
template<>
template<>
void testObj::test<1>(void)
{
  q_.push( ServoName{'a'} );
}

// test push/pop
template<>
template<>
void testObj::test<2>(void)
{
  q_.push( ServoName{'a'} );
  PromisesQueue::Element e=q_.pop();
  ensure_equals("invalid servo name", e.s_.getName(), 'a');
}

// test if order is correct
template<>
template<>
void testObj::test<3>(void)
{
  q_.push( ServoName{'a'} );
  q_.push( ServoName{'b'} );
  PromisesQueue::Element a=q_.pop();
  PromisesQueue::Element b=q_.pop();
  ensure_equals("invalid servo name / 1", a.s_.getName(), 'a');
  ensure_equals("invalid servo name / 2", b.s_.getName(), 'b');
}

// test if future/promise pair works
template<>
template<>
void testObj::test<4>(void)
{
  Response::Future       f=q_.push( ServoName{'a'} );
  PromisesQueue::Element e=q_.pop();
  e.p_.set_value("xyz");
  ensure_equals("invalid element received", f.get(), "xyz");
}

// test getting after queue has been destroyed
template<>
template<>
void testObj::test<5>(void)
{
  std::unique_ptr<PromisesQueue> pq{new PromisesQueue};
  Response::Future f=pq->push( ServoName{'a'} );
  pq.reset();
  try
  {
    f.get();    // should throw
    fail("no exception on broken promise");
  }
  catch(const std::future_error&)
  {
    // this is expected
  }
}

} // namespace tut
