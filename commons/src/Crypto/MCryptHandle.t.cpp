#include <tut/tut.hpp>
#include <string>

#include "Crypto/MCryptHandle.hpp"

using namespace Crypto;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/MCryptHandle");
} // unnamed namespace


namespace tut
{

// test init/deinit
template<>
template<>
void testObj::test<1>(void)
{
  MCryptHandle h(MCRYPT_RIJNDAEL_256, MCRYPT_CFB);
}

// test multiple init/deinit
template<>
template<>
void testObj::test<2>(void)
{
  for(int i=0; i<4; ++i)
    MCryptHandle h(MCRYPT_RIJNDAEL_256, MCRYPT_CFB);
}

// test moving
template<>
template<>
void testObj::test<3>(void)
{
  MCryptHandle h1(MCRYPT_RIJNDAEL_256, MCRYPT_CFB);
  MCryptHandle h2( std::move(h1) );         // cp c-tor
  ensure("cp c-ton failed /1", h1.get()==MCRYPT_FAILED );
  ensure("cp c-ton failed /2", h2.get()!=MCRYPT_FAILED );
  h1 = std::move(h2);                       // cp assignment
  ensure("assignment failed /1", h2.get()==MCRYPT_FAILED );
  ensure("assignment failed /2", h1.get()!=MCRYPT_FAILED );
}

// test getting algo name and mode
template<>
template<>
void testObj::test<4>(void)
{
  MCryptHandle h1(MCRYPT_RIJNDAEL_256, MCRYPT_CFB);
  ensure_equals("invalid algo", h1.algo(), std::string(MCRYPT_RIJNDAEL_256) );
  ensure_equals("invalid mode", h1.mode(), std::string(MCRYPT_CFB) );
}

// ensure member-swap works
template<>
template<>
void testObj::test<5>(void)
{
  MCryptHandle h1(MCRYPT_RIJNDAEL_256, MCRYPT_CFB);
  MCryptHandle h2(MCRYPT_RIJNDAEL_256, MCRYPT_CFB);
  MCRYPT p1 = h1.get();
  MCRYPT p2 = h2.get();
  h1.swap(h2);
  ensure("invalid pointer 1", h1.get()==p2 );
  ensure("invalid pointer 2", h2.get()==p1 );
}

// test non-member swap
template<>
template<>
void testObj::test<6>(void)
{
  MCryptHandle h1(MCRYPT_RIJNDAEL_256, MCRYPT_CFB);
  MCryptHandle h2(MCRYPT_RIJNDAEL_256, MCRYPT_CFB);
  MCRYPT p1 = h1.get();
  MCRYPT p2 = h2.get();
  swap(h1, h2);
  ensure("invalid pointer 1", h1.get()==p2 );
  ensure("invalid pointer 2", h2.get()==p1 );
}

} // namespace tut
