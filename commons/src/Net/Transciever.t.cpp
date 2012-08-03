#include <tut/tut.hpp>

#include "Net/Transciever.hpp"

using namespace Net;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Net/Transciever");
} // unnamed namespace


namespace tut
{

// 
template<>
template<>
void testObj::test<1>(void)
{
}

// 
template<>
template<>
void testObj::test<2>(void)
{
}


// 
template<>
template<>
void testObj::test<3>(void)
{
}

} // namespace tut
