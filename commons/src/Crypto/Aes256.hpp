#ifndef INCLUDE_CRYPTO_AES256_HPP_FILE
#define INCLUDE_CRYPTO_AES256_HPP_FILE

#include <stdexcept>
#include <mcrypt.h>

#include "Crypto/Algo.hpp"
#include "Crypto/MCryptHandle.hpp"

namespace Crypto
{

class Aes256: public Algo
{
public:
  explicit Aes256(BinData key);
  virtual ~Aes256(void);

  virtual const char* name(void) const;
  virtual const char* mode(void) const;
  virtual size_t blockSize(void) const;

  static size_t keySize(void);
  static size_t ivSize(void);

private:
  virtual void encryptImpl(uint8_t* data, size_t size);
  virtual void decryptImpl(uint8_t* data, size_t size);

  BinData      key_;
  BinData      iv_;
  MCryptHandle td_;
};

}

#endif
