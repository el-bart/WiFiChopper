#include <tut/tut.hpp>
#include <exception>
#include <string>

#include "ServoCtrl/Response.hpp"

using namespace ServoCtrl;

namespace
{
struct TestClass
{
  TestClass(void):
    p_{},
    r_{ p_.get_future() }
  {
  }

  Response::Promise p_;
  Response          r_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ServoCtrl/Response");
} // unnamed namespace


namespace tut
{

// try getting non-error reponse with ok()
template<>
template<>
void testObj::test<1>(void)
{
  p_.set_value("ok");
  ensure("invalid reponse", r_.ok() );
}

// try getting non-error reponse with error()
template<>
template<>
void testObj::test<2>(void)
{
  p_.set_value("ok");
  ensure("invalid reponse", r_.error()==false );
}

// test i exceptions are not propagated by ok()
template<>
template<>
void testObj::test<3>(void)
{
  std::exception_ptr ptr{ copy_exception( std::runtime_error{"test exception"} ) };
  p_.set_exception(ptr);
  ensure("invalid reponse", r_.ok()==false );
}

// test i exceptions are not propagated by error()
template<>
template<>
void testObj::test<4>(void)
{
  std::exception_ptr ptr{ copy_exception( std::runtime_error{"test exception"} ) };
  p_.set_exception(ptr);
  ensure("invalid reponse", r_.error() );
}

// test multiple calls to ok()
template<>
template<>
void testObj::test<5>(void)
{
  p_.set_value("ok");
  ensure("invalid response / 1", r_.ok() );
  ensure("invalid response / 2", r_.ok() );
}

// test multiple calls to error()
template<>
template<>
void testObj::test<6>(void)
{
  p_.set_value("ok");
  ensure("invalid response / 1", r_.error()==false );
  ensure("invalid response / 2", r_.error()==false );
}

// test multiple calls to getError()
template<>
template<>
void testObj::test<7>(void)
{
  std::exception_ptr ptr{ copy_exception( std::runtime_error{"test exception"} ) };
  p_.set_exception(ptr);

  ptr=r_.getError();
  if(!ptr)
    fail("null pointer received / 1");
  ptr=r_.getError();
  if(!ptr)
    fail("null pointer received / 2");
}

// test getting error
template<>
template<>
void testObj::test<8>(void)
{
  std::exception_ptr ptr{ copy_exception( std::runtime_error{"test exception"} ) };
  p_.set_exception(ptr);
  ptr=r_.getError();
  if(!ptr)
    fail("null pointer received");
  try
  {
    std::rethrow_exception(ptr);
  }
  catch(const std::runtime_error &ex)
  {
    ensure_equals("invalid error message", ex.what(), std::string{"test exception"} );
  }
}

// test getting error when none was detected
template<>
template<>
void testObj::test<9>(void)
{
  p_.set_value("ok");
  if(r_.getError())
    fail("exception set, while no error risen");
}

// test error message on no error
template<>
template<>
void testObj::test<10>(void)
{
  p_.set_value("ok");
  ensure_equals("invalid message", r_.getErrorMessage(), "no error");
}

// test error message on protocol error
template<>
template<>
void testObj::test<11>(void)
{
  p_.set_value("x-ERR");
  ensure_equals("invalid message", r_.getErrorMessage(), "protocol reported an error: x-ERR");
}

// test error message on exception
template<>
template<>
void testObj::test<12>(void)
{
  std::exception_ptr ptr{ copy_exception( std::runtime_error{"my exception"} ) };
  p_.set_exception(ptr);
  ensure_equals("invalid message", r_.getErrorMessage(), "my exception");
}

} // namespace tut
