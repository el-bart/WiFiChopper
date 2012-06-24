#include <tut/tut.hpp>

#include "FastQueue.hpp"

namespace
{
struct TestClass
{
  FastQueue<3> q_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("FastQueue");
} // unnamed namespace


namespace tut
{

// test size of a newly created queue
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid initial size", q_.size(), 0);
}

// test adding one element
template<>
template<>
void testObj::test<2>(void)
{
  q_.push(42);
  ensure_equals("invalid size", q_.size(), 1);
  ensure_equals("invalid element", q_.peek(0), 42);
}

// test getting element from stack
template<>
template<>
void testObj::test<3>(void)
{
  q_.push(42);
  ensure_equals("invalid element", q_.pop(), 42);
  ensure_equals("invalid size after removal", q_.size(), 0);
}

// test copyability
template<>
template<>
void testObj::test<4>(void)
{
  q_.push(6);
  q_.push(9);
  FastQueue<3> q=q_;
  q_.clear();
  ensure_equals("invalid size of copyied queue", q.size(), 2);
  ensure_equals("invalid element 0", q.peek(0), 6);
  ensure_equals("invalid element 1", q.peek(1), 9);
}

// test clear
template<>
template<>
void testObj::test<5>(void)
{
  q_.push(6);
  q_.push(9);
  q_.clear();
  ensure_equals("invalid size after cleaning", q_.size(), 0);
}

// test peeking multiple elements
template<>
template<>
void testObj::test<6>(void)
{
  q_.push(6);
  q_.push(9);
  ensure_equals("invalid size", q_.size(), 2);
  ensure_equals("invalid element 0", q_.peek(0), 6);
  ensure_equals("invalid element 1", q_.peek(1), 9);
}

// test content after adding and removing few elements
template<>
template<>
void testObj::test<7>(void)
{
  q_.push(6);
  q_.push(9);
  q_.push(3);
  q_.pop();
  ensure_equals("invalid size", q_.size(), 2);
  ensure_equals("invalid element 0", q_.peek(0), 9);
  ensure_equals("invalid element 1", q_.peek(1), 3);
}

// test popping after adding and removing few elements
template<>
template<>
void testObj::test<8>(void)
{
  q_.push(6);
  q_.push(9);
  q_.push(3);
  q_.pop();
  ensure_equals("invalid size/0", q_.size(), 2);
  ensure_equals("invalid element 0", q_.pop(), 9);
  ensure_equals("invalid size/1", q_.size(), 1);
  ensure_equals("invalid element 1", q_.pop(), 3);
  ensure_equals("invalid size/2", q_.size(), 0);
}

} // namespace tut
