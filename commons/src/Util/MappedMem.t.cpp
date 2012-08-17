#include <tut/tut.hpp>
#include <algorithm>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#include "Util/MappedMem.hpp"
#include "Util/UniqueDescriptor.hpp"

using namespace Util;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/MappedMem");
} // unnamed namespace


namespace tut
{

// try mmaping some data
template<>
template<>
void testObj::test<1>(void)
{
  UniqueDescriptor fd( open("/etc/fstab", O_RDONLY) );
  ensure("cannot open test file", fd.get() != -1 );
  MappedMem<char> mm(10, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd.get(), 0);
  ensure_equals("invalid map size", mm.size(), 10);
  ensure("memory is NULL", mm.mem() != nullptr );
  for(size_t i=0; i<mm.size(); ++i)
    ensure("char is notnprintable - invalid mapping?", isprint( mm.mem()[i] ) );
}


// test movability (c-tor)
template<>
template<>
void testObj::test<2>(void)
{
  UniqueDescriptor fd( open("/etc/fstab", O_RDONLY) );
  ensure("cannot open test file", fd.get() != -1 );
  MappedMem<char> mm(10, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd.get(), 0);
  // test mv-ctor
  MappedMem<char> other( std::move(mm) );
  ensure("ownership not transffered", mm.mem() == nullptr );
  ensure("ownership lost", other.mem() != nullptr );
}


// test void*
template<>
template<>
void testObj::test<3>(void)
{
  UniqueDescriptor fd( open("/etc/fstab", O_RDONLY) );
  ensure("cannot open test file", fd.get() != -1 );
  MappedMem<void> mm(10, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd.get(), 0);
}


// test movability (assignment)
template<>
template<>
void testObj::test<4>(void)
{
  UniqueDescriptor fd( open("/etc/fstab", O_RDONLY) );
  ensure("cannot open test file", fd.get() != -1 );
  MappedMem<char> mm(10, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd.get(), 0);
  // test mv-assignment
  UniqueDescriptor tmpFd( open("/etc/inittab", O_RDONLY) );
  ensure("cannot open test file", tmpFd.get() != -1 );
  MappedMem<char> tmp(10, PROT_READ|PROT_WRITE, MAP_PRIVATE, tmpFd.get(), 0);
  tmp = std::move(mm);
  ensure("ownership not transffered", mm.mem() == nullptr );
  ensure("ownership lost", tmp.mem() != nullptr );
}


namespace
{
int  g_mmapTestCalls;
int  g_munmapTestCalls;
char g_mmText[] = "alice has a cat";

void *mmapCallTest(void*, size_t, int, int, int, off_t)
{
  ++g_mmapTestCalls;
  return g_mmText;
}

int munmapCallTest(void*, size_t)
{
  ++g_munmapTestCalls;
  return 0;
}
} // unnamed namespace

// try custom m(un)map() calls
template<>
template<>
void testObj::test<5>(void)
{
  g_mmapTestCalls   = 0;
  g_munmapTestCalls = 0;
  typedef MappedMem<char, mmapCallTest, munmapCallTest> MMTest;
  {
    MMTest mm(10, PROT_READ|PROT_WRITE, MAP_PRIVATE, 42, 0);
    ensure_equals("test-mmap() not called", g_mmapTestCalls, 1);
    ensure_equals("test-munmap() called", g_munmapTestCalls, 0);
    ensure_equals("invalid data saved", mm.mem(), std::string(g_mmText) );
  }
  ensure_equals("test-mmap() called again", g_mmapTestCalls, 1);
  ensure_equals("test-munmap() not called", g_munmapTestCalls, 1);
}

} // namespace tut
