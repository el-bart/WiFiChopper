#include <tut/tut.hpp>

#include "Queue.hpp"

namespace
{
struct TestClass
{
  Queue<3> q_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Queue");
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

// test removal element when no element in the queue
template<>
template<>
void testObj::test<4>(void)
{
  q_.pop();
  ensure_equals("invalid size after removal", q_.size(), 0);
}

// test removing multiple elements
template<>
template<>
void testObj::test<5>(void)
{
  q_.push(4);
  q_.push(6);
  ensure_equals("invalid size after addition", q_.size(), 2);
  q_.clear();
  ensure_equals("invalid size after clearing content", q_.size(), 0);
}

// test adding element and removing, so that queue will "rotate"
template<>
template<>
void testObj::test<6>(void)
{
  q_.push(1);
  q_.push(2);
  q_.push(3);
  q_.pop();
  q_.pop();
  q_.push(4);
  ensure_equals("invalid size", q_.size(), 2);
  ensure_equals("invalid element 0", q_.peek(0), 3);
  ensure_equals("invalid element 1", q_.peek(1), 4);
}

// test adding more elements that can be fit into memory
template<>
template<>
void testObj::test<7>(void)
{
  q_.push(1);
  q_.push(2);
  q_.push(3);
  q_.push(4);
  ensure_equals("invalid size", q_.size(), 3);
  ensure_equals("invalid element 0", q_.peek(0), 2);
  ensure_equals("invalid element 1", q_.peek(1), 3);
  ensure_equals("invalid element 2", q_.peek(2), 4);
}

// test popping when queue "rotated"
template<>
template<>
void testObj::test<8>(void)
{
  q_.push(1);
  q_.push(2);
  q_.push(3);
  q_.push(4);
  ensure_equals("invalid element poped 0", q_.pop(), 2);
  ensure_equals("invalid element poped 1", q_.pop(), 3);
  ensure_equals("invalid element poped 2", q_.pop(), 4);
}

// test removing multiple elements
template<>
template<>
void testObj::test<9>(void)
{
  q_.push(1);
  q_.push(2);
  q_.push(3);
  q_.remove(2);
  ensure_equals("invalid size", q_.size(), 1);
  ensure_equals("invalid element 1", q_.peek(0), 3);
}

// test removing more element than present
template<>
template<>
void testObj::test<10>(void)
{
  q_.push(1);
  q_.remove(2);
  ensure_equals("invalid size", q_.size(), 0);
}

// test removing more elements that can be insterted
template<>
template<>
void testObj::test<11>(void)
{
  q_.push(1);
  q_.remove(4);
  ensure_equals("invalid size", q_.size(), 0);
}

// test removing when queue "rotated"
template<>
template<>
void testObj::test<12>(void)
{
  q_.push(1);
  q_.push(2);
  q_.push(3);
  q_.push(4);
  q_.remove(1);
  ensure_equals("invalid size", q_.size(), 2);
  ensure_equals("invalid element 0", q_.peek(0), 3);
  ensure_equals("invalid element 1", q_.peek(1), 4);
}

// test copyability
template<>
template<>
void testObj::test<13>(void)
{
  q_.push(1);
  q_.push(2);
  q_.push(3);
  q_.push(4);
  Queue<3> q=q_;
  q_.clear();
  ensure_equals("invalid size of copyied queue", q.size(), 3);
  ensure_equals("invalid element 0", q.peek(0), 2);
  ensure_equals("invalid element 1", q.peek(1), 3);
  ensure_equals("invalid element 2", q.peek(2), 4);
}

} // namespace tut
