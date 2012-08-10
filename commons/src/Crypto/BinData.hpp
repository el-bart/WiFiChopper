#ifndef INCLUDE_CRYPTO_BINDATA_HPP_FILE
#define INCLUDE_CRYPTO_BINDATA_HPP_FILE

#include <vector>
#include <cinttypes>

namespace Crypto
{

class BinData
{
public:
  explicit BinData(const size_t size):
    d_(size)
  { }

  uint8_t* data(void) { return d_.data(); }
  const uint8_t* data(void) const { return d_.data(); }

  size_t size(void) const { return d_.size(); }

private:
  typedef std::vector<uint8_t> Data;
  Data d_;
};

}

#endif
