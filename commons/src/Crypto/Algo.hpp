#ifndef INCLUDE_CRYPTO_ALGO_HPP_FILE
#define INCLUDE_CRYPTO_ALGO_HPP_FILE

#include <vector>
#include <random>
#include <stdexcept>
#include <functional>
#include <cinttypes>
#include <cassert>

#include "Crypto/BinData.hpp"

namespace Crypto
{

class Algo
{
public:
  typedef std::vector<uint8_t> Data;

  Algo(void);
  virtual ~Algo(void) { }

  // noncopyable
  Algo(const Algo&) = delete;
  Algo& operator=(const Algo&) = delete;
  // nonmovable
  Algo(Algo&&) = delete;
  Algo& operator=(Algo&&) = delete;

  virtual const char* name(void) const = 0;
  virtual const char* mode(void) const = 0;
  virtual size_t blockSize(void) const = 0;

  void encrypt(Data& buf)
  {
    addPadding(buf);
    assert( buf.size()%blockSize() == 0 );
    encryptImpl( buf.data(), buf.size() );
  }

  void decrypt(Data& buf)
  {
    if( buf.size()%blockSize() != 0 )
      throw std::runtime_error("invalid padding");
    if( buf.size()<2*blockSize() )
      throw std::runtime_error("not enough data in the buffer");
    decryptImpl( buf.data(), buf.size() );
    removePadding(buf);
  }

private:
  void addPadding(Data& buf) const;
  void removePadding(Data& buf) const;

  virtual void encryptImpl(uint8_t* data, size_t size) = 0;
  virtual void decryptImpl(uint8_t* data, size_t size) = 0;

  std::random_device                     rndDev_;
  std::uniform_int_distribution<uint8_t> dist_;
  std::mt19937                           eng_;
  std::function<uint8_t(void)>           rand_;
};

}

#endif
