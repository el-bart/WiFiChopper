#ifndef INCLUDE_UTIL_UNIQUEDESCRIPTOR_HPP_FILE
#define INCLUDE_UTIL_UNIQUEDESCRIPTOR_HPP_FILE

#include <utility>
#include <unistd.h>

namespace Util
{

class UniqueDescriptor
{
public:
  explicit UniqueDescriptor(const int desc):
    desc_(desc)
  { }

  ~UniqueDescriptor(void)
  {
    if(desc_!=-1)
      close(desc_);
  }

  // movable
  UniqueDescriptor(UniqueDescriptor&& other):
    desc_(other.desc_)
  {
    other.desc_=-1;
  }

  UniqueDescriptor& operator=(UniqueDescriptor&& other)
  {
    UniqueDescriptor tmp( std::move(other) );
    swap(tmp);
    return *this;
  }

  // non-copyable
  UniqueDescriptor(const UniqueDescriptor&) = delete;
  UniqueDescriptor& operator=(const UniqueDescriptor&) = delete;

  int get(void) const { return desc_; }

  void swap(UniqueDescriptor& other)
  {
    std::swap(desc_, other.desc_);
  }

private:
  int desc_;
};

}

#endif
