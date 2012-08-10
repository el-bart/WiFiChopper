#include <tut/tut.hpp>

#include "Util/StrictWeakOrdering.hpp"

using namespace Util;

namespace
{

struct Number: public StrictWeakOrdering<Number>
{
  explicit Number(int n): n_(n) { }
  bool operator<(const Number& rhs) const { return n_<rhs.n_; }
  int n_;
};

struct TestClass
{
  TestClass(void):
    n13_(13),
    n42_(42)
  { }

  Number n13_;
  Number n42_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Util/StrictWeakOrdering");
} // unnamed namespace


namespace tut
{

// test < (sanity check)
template<>
template<>
void testObj::test<1>(void)
{
  ensure("operator < failed - positive",   n13_<n42_  );
  ensure("operator < failed - negative", !(n42_<n13_) );
}

// test >
template<>
template<>
void testObj::test<2>(void)
{
  ensure("operator > failed - positive",   n42_>n13_  );
  ensure("operator > failed - negative", !(n13_>n42_) );
}

// test ==
template<>
template<>
void testObj::test<3>(void)
{
  ensure("operator == failed - positive",     n13_==n13_  );
  ensure("operator == failed - negative 1", !(n13_==n42_) );
  ensure("operator == failed - negative 2", !(n42_==n13_) );
}

// test !=
template<>
template<>
void testObj::test<4>(void)
{
  ensure("operator != failed - positive",   n42_!=n13_  );
  ensure("operator != failed - negative", !(n13_!=n13_) );
}

// test <=
template<>
template<>
void testObj::test<5>(void)
{
  ensure("operator <= failed - positive 1",   n13_<=n42_  );
  ensure("operator <= failed - positive 2",   n13_<=n13_  );
  ensure("operator <= failed - negative",   !(n42_<=n13_) );
}

// test >=
template<>
template<>
void testObj::test<6>(void)
{
  ensure("operator >= failed - positive 1",   n42_>=n13_  );
  ensure("operator >= failed - positive 2",   n13_>=n13_  );
  ensure("operator >= failed - negative",   !(n13_>=n42_) );
}

} // namespace tut
