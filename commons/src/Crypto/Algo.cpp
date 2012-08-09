#include "Crypto/Algo.hpp"

namespace Crypto
{

Algo::Algo(void):
  dist_(0, 0xFF),
  eng_( rndDev_() ),
  rand_( [this]{ return this->dist_(this->eng_); } )
{
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
    for(auto& b: randJunk)
      b = rand_();
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
