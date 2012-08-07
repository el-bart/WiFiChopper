/*
 * SharedPtrNotNull.hpp
 *
 * code taken from System library:
 * http://baszerr.org/doku.php/prjs/system/system
 *
 */
#ifndef INCLUDE_SERVOCTRL_SHAREDPTRNOTNULL_HPP_FILE
#define INCLUDE_SERVOCTRL_SHAREDPTRNOTNULL_HPP_FILE

/* public header */

#include <memory>
#include <cassert>

#include "ServoCtrl/Exception.hpp"

namespace ServoCtrl
{

/** \brief std::shared_ptr-like class that does not permit Nulls.
 *  \note if Null will be passed to this class it will throw ExceptionNullPointer.
 */
template<typename T>
class SharedPtrNotNull
{
private:
  typedef std::shared_ptr<T>  SharedPtr;
  typedef SharedPtrNotNull<T> this_type;
  typedef T*                  pointer;
  typedef T&                  reference;

public:
  /** \brief exception thrown when trying to assign nullptr.
   */
  struct ExceptionNullPointer: public Exception
  {
    /** \brief create error message.
     */
    ExceptionNullPointer(void):
      Exception("trying to pass nullptr to SharedPtrNotNull")
    {
    }
  }; // struct ExceptionNullPointer

  /** \brief type of element held inside (for compatibility with std::shared_ptr). */
  typedef typename SharedPtr::element_type element_type;

  /** \brief create class from raw-pointer.
   *  \param t pointer to take ownership of.
   */
  explicit SharedPtrNotNull(pointer t):
    ptr_(t)
  {
    ensure();
  }
  /** \brief move c-tor.
   *  \param other object to move from.
   */
  SharedPtrNotNull(SharedPtrNotNull<T> &&other)
  {
    ptr_=std::move(other.ptr_);
    assert( ptr_.get()!=nullptr );
  }
  /** \brief copy c-tor.
   *  \param other object to move from.
   */
  SharedPtrNotNull(const this_type &other)
  {
    ptr_=other.ptr_;
    assert( ptr_.get()!=nullptr );
  }
  /** \brief conversion c-tor (from related pointers).
   *  \param other object to copy from.
   */
  template<typename U>
  SharedPtrNotNull(const SharedPtrNotNull<U> &other)
  {
    ptr_=other.shared_ptr();
    assert( ptr_.get()!=nullptr );
  }
  /** \brief create object from std::shared_ptr<>.
   *  \param p pointer to share.
   */
  template<typename U>
  SharedPtrNotNull(const std::shared_ptr<U> &p):
    ptr_(p)
  {
    ensure();
  }
  /** \brief create object from std::unique_ptr<>.
   *  \param p pointer get ownership of.
   */
  template<typename U>
  SharedPtrNotNull(std::unique_ptr<U> p):
    ptr_( p.release() )
  {
    ensure();
    assert( p.get()==nullptr );
  }
  /** \brief conversion to std::shared_ptr<>.
   *  \return std::shared_ptr<> for a given value.
   */
  SharedPtr shared_ptr(void) const
  {
    assert( ptr_.get()!=nullptr );
    return ptr_;
  }
  /** \brief move-assignment of other instance.
   *  \param other object to assign from.
   *  \return reference to this object.
   */
  this_type& operator=(this_type &&other)
  {
    if( other.get()!=this->get() )
    {
      this_type tmp( std::move(other) );
      tmp.swap(*this);
    }
    return *this;
  }
  /** \brief assignment of other instance.
   *  \param other object to assign from.
   *  \return reference to this object.
   */
  this_type& operator=(const this_type &other)
  {
    if( other.get()!=this->get() )
      ptr_=other.shared_ptr();
    return *this;
  }
  /** \brief assignment of other instance.
   *  \param other object to assign from.
   *  \return reference to this object.
   */
  template<typename U>
  this_type& operator=(const SharedPtrNotNull<U> &other)
  {
    if( other.get()!=this->get() )
      ptr_=other.shared_ptr();
    return *this;
  }
  /** \brief arror operator.
   *  \return pointer to this.
   */
  pointer operator->(void) const
  {
    return get();
  }
  /** \brief dereference operator.
   *  \return reference to held object.
   */
  reference operator*(void) const
  {
    return *get();
  }
  /** \brief test const getter.
   *  \return direct pointer value.
   */
  pointer get(void) const
  {
    assert( ptr_.get()!=nullptr );
    return ptr_.get();
  }
  /** \brief swaps pointers held inside.
   *  \param other object to swap pointers with.
   *  \note this call is NOT thread safe, and make others not-thread-safe too!
   *        if this call is to be used, whole access has to be carefully
   *        mutexed, along with all places given object is used in.
   */
  void swap(this_type &other)
  {
    assert( get()!=nullptr );
    ptr_.swap(other.ptr_);
    assert( get()!=nullptr );
  }

private:
  void ensure(void) const
  {
    ensure( ptr_.get() );
  }
  void ensure(const pointer t) const
  {
    if(t==nullptr)
      throw ExceptionNullPointer();
  }

  SharedPtr ptr_;
}; // struct SharedPtrNotNull




//
// NOTE: all operators must be externally defined in order to avoid disambiguation
//       during calls like SPNN<X> == SPNN<const X>.
//

/** \brief compare pointers.
 *  \param left  left side of the relation.
 *  \param right right side of the relation.
 *  \return result of corresponding operation on raw pointers.
 */
template<typename T, typename U>
bool operator<(const SharedPtrNotNull<T> &left, const SharedPtrNotNull<U> &right)
{
  return left.get()<right.get();
}

/** \brief compare pointers.
 *  \param left  left side of the relation.
 *  \param right right side of the relation.
 *  \return result of corresponding operation on raw pointers.
 */
template<typename T, typename U>
bool operator==(const SharedPtrNotNull<T> &left, const SharedPtrNotNull<U> &right)
{
  return left.get()==right.get();
}

/** \brief compare pointers.
 *  \param left  left side of the relation.
 *  \param right right side of the relation.
 *  \return result of corresponding operation on raw pointers.
 */
template<typename T, typename U>
bool operator!=(const SharedPtrNotNull<T> &left, const SharedPtrNotNull<U> &right)
{
  return left.get()!=right.get();
}

/** \brief compare pointers.
 *  \param left  left side of the relation.
 *  \param right right side of the relation.
 *  \return result of corresponding operation on raw pointers.
 */
template<typename T, typename U>
bool operator<(const std::shared_ptr<T> &left, const SharedPtrNotNull<U> &right)
{
  return left.get()<right.get();
}

/** \brief compare pointers.
 *  \param left  left side of the relation.
 *  \param right right side of the relation.
 *  \return result of corresponding operation on raw pointers.
 */
template<typename T, typename U>
bool operator==(const std::shared_ptr<T> &left, const SharedPtrNotNull<U> &right)
{
  return left.get()==right.get();
}

/** \brief compare pointers.
 *  \param left  left side of the relation.
 *  \param right right side of the relation.
 *  \return result of corresponding operation on raw pointers.
 */
template<typename T, typename U>
bool operator!=(const std::shared_ptr<T> &left, const SharedPtrNotNull<U> &right)
{
  return left.get()!=right.get();
}

/** \brief compare pointers.
 *  \param left  left side of the relation.
 *  \param right right side of the relation.
 *  \return result of corresponding operation on raw pointers.
 */
template<typename T, typename U>
bool operator<(const SharedPtrNotNull<T> &left, const std::shared_ptr<U> &right)
{
  return left.get()<right.get();
}

/** \brief compare pointers.
 *  \param left  left side of the relation.
 *  \param right right side of the relation.
 *  \return result of corresponding operation on raw pointers.
 */
template<typename T, typename U>
bool operator==(const SharedPtrNotNull<T> &left, const std::shared_ptr<U> &right)
{
  return left.get()==right.get();
}

/** \brief compare pointers.
 *  \param left  left side of the relation.
 *  \param right right side of the relation.
 *  \return result of corresponding operation on raw pointers.
 */
template<typename T, typename U>
bool operator!=(const SharedPtrNotNull<T> &left, const std::shared_ptr<U> &right)
{
  return left.get()!=right.get();
}

} // namespace ServoCtrl

#endif
