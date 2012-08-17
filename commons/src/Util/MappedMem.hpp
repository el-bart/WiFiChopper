#ifndef INCLUDE_UTIL_MAPPEDMEM_HPP_FILE
#define INCLUDE_UTIL_MAPPEDMEM_HPP_FILE

#include <algorithm>
#include <cinttypes>
#include <cstring>
#include <cerrno>
#include <sys/mman.h>

#include "Util/Exception.hpp"


namespace Util
{

template<typename T, void *(*mmapCall)(void*, size_t, int, int, int, off_t)=mmap, int (*munmapCall)(void*, size_t)=munmap>
class MappedMem
{
public:
  MappedMem(const unsigned size, const int prot, const int flags, const int fd, const off_t offset):
    size_( size ),
    mem_( static_cast<T*>( (*mmapCall)( nullptr /* anywhere */, size_, prot, flags, fd, offset ) ) )
  {
    if( mem_ == MAP_FAILED )
      throw Util::Exception( UTIL_LOCSTRM << "mmap() failed: " << strerror(errno) );
  }

  ~MappedMem(void)
  {
    if( mem_ != nullptr )
      (*munmapCall)( mem_, size_ );
  }

  MappedMem(MappedMem&& other):
    size_( other.size_ ),
    mem_( other.mem_ )
  {
    other.size_ = 0;
    other.mem_  = nullptr;
  }

  MappedMem& operator=(MappedMem&& other)
  {
    MappedMem tmp( std::move(other) );
    std::swap( size_, tmp.size_ );
    std::swap( mem_,  tmp.mem_  );
    return *this;
  }

  size_t size(void) const { return size_; }
  T* mem(void) { return mem_; }
  const T* mem(void) const { return mem_; }

private:
  size_t size_;
  T*     mem_;
};

}

#endif
