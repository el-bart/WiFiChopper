#include <tut/tut.hpp>

#include "Util/readWholeFile.hpp"

using namespace Util;

namespace
{
struct TestClass
{
  typedef std::vector<uint8_t> Content;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/readWholeFile");
} // unnamed namespace


namespace tut
{

// reading some random file
template<>
template<>
void testObj::test<1>(void)
{
  const Content c = readWholeFile(__FILE__);
  ensure("too short", c.size() > __LINE__*10);
}


// test reading non-existing file
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    readWholeFile("/non/existing/path");
    fail("no exception uppon non-existing file");
  }
  catch(const std::runtime_error&)
  { }
}


// test reading file w/o proper permisions
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    readWholeFile("/proc/kmsg");
    fail("no exception uppon file that cannot be read");
  }
  catch(const std::runtime_error&)
  { }
}

} // namespace tut
