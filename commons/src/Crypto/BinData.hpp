#ifndef INCLUDE_CRYPTO_BINDATA_HPP_FILE
#define INCLUDE_CRYPTO_BINDATA_HPP_FILE

#include <memory>
#include <stdexcept>
#include <cinttypes>

namespace Crypto
{

class BinData
{
public:
  explicit BinData(const size_t size):
    d_(new uint8_t[size]),
    size_(size)
  {
    if( d_.get()==nullptr )
      throw std::runtime_error("BinData: unable to allocate memory");
  }

  uint8_t* data(void) { return d_.get(); }
  const uint8_t* data(void) const { return d_.get(); }

  size_t size(void) const { return size_; }

private:
  typedef std::unique_ptr<uint8_t[]> Data;

  Data   d_;
  size_t size_;
};

}

#endif
