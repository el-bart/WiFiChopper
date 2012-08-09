#include <algorithm>
#include <cassert>

#include "Crypto/Aes256.hpp"
#include "Util/ErrStrm.hpp"

namespace Crypto
{

Aes256::Aes256(BinData key):
  key_( std::move(key) ),
  iv_( generateRandomData( ivSize() ) ),
  td_(MCRYPT_RIJNDAEL_256, MCRYPT_CFB)
{
  if( key_.size()!=iv_.size() )
    throw std::runtime_error("key and IV size differ");

  assert( static_cast<size_t>(mcrypt_enc_get_key_size(td_.get()))==keySize() );
  assert( static_cast<size_t>(mcrypt_enc_get_iv_size(td_.get())) ==ivSize()  );

  int ret;
  if( (ret = mcrypt_generic_init(td_.get(), key_.data(), key_.size(), iv_.data())) < 0 )
    throw std::runtime_error( (Util::ErrStrm{}<<"mcrypt_generic_init(): "<<mcrypt_strerror(ret)).str().c_str() );
}

Aes256::~Aes256(void)
{
  mcrypt_generic_deinit( td_.get() );
}

const char* Aes256::name(void) const
{
  return MCRYPT_RIJNDAEL_256;
}

const char* Aes256::mode(void) const
{
  return MCRYPT_CFB;
}

size_t Aes256::blockSize(void) const
{
  return keySize();
}

size_t Aes256::keySize(void)
{
  return 256/8;
}

size_t Aes256::ivSize(void)
{
  return 256/8;
}

void Aes256::encryptImpl(uint8_t* data, size_t size)
{
  int ret;
  if( (ret = mcrypt_generic( td_.get(), data, size )) < 0 )
    throw std::runtime_error( (Util::ErrStrm{}<<"mcrypt_generic(): "<<mcrypt_strerror(ret)).str().c_str() );
}

void Aes256::decryptImpl(uint8_t* data, size_t size)
{
  int ret;
  if( (ret = mdecrypt_generic( td_.get(), data, size )) < 0 )
    throw std::runtime_error( (Util::ErrStrm{}<<"mdecrypt_generic(): "<<mcrypt_strerror(ret)).str().c_str() );
}

}
