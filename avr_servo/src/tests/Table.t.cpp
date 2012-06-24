#include <tut/tut.hpp>
#include <string>

#include "Table.hpp"

using std::string;

namespace
{
struct TestClass
{
  Table<string,3> t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Table");
} // unnamed namespace


namespace tut
{

// test writing some elements
template<>
template<>
void testObj::test<1>(void)
{
  t_[2]="kszy";
  ensure_equals("invalid element", t_[2], "kszy");
}

// test copyability
template<>
template<>
void testObj::test<2>(void)
{
  t_[0]="zero";
  t_[1]="one";
  t_[2]="two";
  Table<string,3> t=t_;
  ensure_equals("invalid element 0", t[0], "zero");
  ensure_equals("invalid element 1", t[1], "one");
  ensure_equals("invalid element 2", t[2], "two");
}

// test value_type typedef existance
template<>
template<>
void testObj::test<3>(void)
{
  Table<string,3>::value_type str("abc");
}

} // namespace tut
