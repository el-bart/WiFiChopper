#ifndef INCLUDE_CRYPTO_ALGO_HPP_FILE
#define INCLUDE_CRYPTO_ALGO_HPP_FILE

#include <cinttypes>

#include "Crypto/BinData.hpp"

namespace Crypto
{

class Algo
{
public:
  Algo(void) { }
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

  void encrypt(const uint8_t* data, size_t size)
  {
    if(data==nullptr)
      throw std::runtime_error("data pointer cannot be null");
    encryptImpl(data, size);
  }

  void decrypt(const uint8_t* data, size_t size)
  {
    if(data==nullptr)
      throw std::runtime_error("data pointer cannot be null");
    decryptImpl(data, size);
  }

private:
  virtual void encryptImpl(const uint8_t* data, size_t size) = 0;
  virtual void decryptImpl(const uint8_t* data, size_t size) = 0;
};

}

#endif
