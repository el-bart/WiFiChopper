#ifndef INCLUDE_CRYPTO_PLAINTEXT_HPP_FILE
#define INCLUDE_CRYPTO_PLAINTEXT_HPP_FILE

#include "Crypto/Algo.hpp"

namespace Crypto
{

class PlainText: public Algo
{
public:
  virtual const char* name(void) const { return "PlainText"; }
  virtual const char* mode(void) const { return "none"; }
  virtual size_t blockSize(void) const { return 1; };

private:
  virtual void encryptImpl(uint8_t* /*data*/, size_t /*size*/) { };
  virtual void decryptImpl(uint8_t* /*data*/, size_t /*size*/) { }
};

}

#endif
