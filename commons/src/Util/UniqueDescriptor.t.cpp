#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <tut/tut.hpp>

#include "Util/UniqueDescriptor.hpp"

using namespace Util;

namespace
{
struct TestClass
{
  UniqueDescriptor make(void) const
  {
    return UniqueDescriptor{ open("/dev/null", O_RDONLY) };
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/UniqueDescriptor");
} // unnamed namespace


namespace tut
{

// test movability
template<>
template<>
void testObj::test<1>(void)
{
  UniqueDescriptor ud=make();
  ensure("invalid descriptor", ud.get() > 0 );

  const int        fd =ud.get();
  UniqueDescriptor ud2=std::move(ud);
  ensure_equals("move failed 1", ud2.get(), fd);
  ensure_equals("move failed 2", ud.get(),  -1);
}


// test movability by copy c-tor
template<>
template<>
void testObj::test<2>(void)
{
  UniqueDescriptor ud=make();
  UniqueDescriptor ud2( std::move(ud) );

  ensure("invalid descriptor", ud2.get() > 0 );
  ensure_equals("move failed ", ud.get(), -1 );
}


// test swap
template<>
template<>
void testObj::test<3>(void)
{
  UniqueDescriptor ud1=make();
  UniqueDescriptor ud2=make();
  const int fd1=ud1.get();
  const int fd2=ud2.get();
  ensure("invalid descriptors returned", fd1!=fd2);
  ud1.swap(ud2);
  ensure_equals("ivalid value 1", ud1.get(), fd2);
  ensure_equals("ivalid value 2", ud2.get(), fd1);
}


// test initializing with -1
template<>
template<>
void testObj::test<4>(void)
{
  UniqueDescriptor ud(-1);
  ensure_equals("ivalid value", ud.get(), -1);
}

} // namespace tut
