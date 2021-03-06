#include "Crypto/Algo.hpp"

namespace Crypto
{

Algo::Algo(void):
  dist_(0, 0xFF),
  eng_( rndDev_() ),
  rand_( [this]{ return this->dist_(this->eng_); } )
{
}


void Algo::encrypt(Data& buf)
{
  addPadding(buf);
  assert( buf.size()%blockSize() == 0 );
  encryptImpl( buf.data(), buf.size() );
}


void Algo::decrypt(Data& buf)
{
  const size_t bs = blockSize();
  if( buf.size()%bs != 0 )
    throw std::runtime_error("invalid padding");
  if( buf.size()<2*bs )
    throw std::runtime_error("not enough data in the buffer");
  decryptImpl( buf.data(), buf.size() );
  removePadding(buf);
}


BinData Algo::generateRandomData(size_t len)
{
  BinData tmp(len);
  for(size_t i=0; i<tmp.size(); ++i)
    tmp.data()[i] = rand_();
  return std::move(tmp);
}


void Algo::addPadding(Data& buf) const
{
  const size_t bs     = blockSize();
  const size_t ending = buf.size() % bs;
  const size_t toPad  = bs-ending;
  // prepare extra memory for this
  buf.reserve( bs + buf.size() + toPad );

  // step 1: add random block at the begin
  {
    uint8_t randJunk[bs];
    for(size_t i=0; i<bs; ++i)
      randJunk[i] = rand_();
    buf.insert( buf.begin(), randJunk, randJunk+bs );
  }

  // step 2: add pading to the integral multiply of block count
  for(size_t i=0; i<toPad; ++i)
    buf.push_back(toPad);
}


void Algo::removePadding(Data& buf) const
{
  const size_t bs = blockSize();

  // step 1: remove end padding
  const size_t padSize = *buf.rbegin();
  if(padSize>bs)
    throw std::runtime_error("invalid pad size - message is broken");
  buf.erase( buf.begin()+buf.size()-padSize, buf.end() );

  // step 2: remove first block
  buf.erase( buf.begin(), buf.begin()+bs );
}

}
