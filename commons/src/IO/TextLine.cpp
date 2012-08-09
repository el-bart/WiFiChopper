#include <iterator>
#include <algorithm>
#include <cassert>

#include "IO/TextLine.hpp"
#include "Util/TimeoutClock.hpp"
#include "Util/tabSize.hpp"

using namespace std;


namespace IO
{

namespace
{

constexpr uint8_t g_eol[] = {'\n'};

template<typename Iter>
Iter findEOL(const Iter begin, const Iter end)
{
  // look for the end of line
  for(Iter it=begin; it!=end; ++it)
    if( *it == g_eol[0] )
      return it;
  // EOL not found
  return end;
}

}


void TextLine::send(const uint8_t *data, const size_t size)
{
  if(data==nullptr)
    throw ParameterError("data pointer cannot be NULL");

  // send data
  size_t done = 0;
  size_t left = size;
  while(left>0)
  {
    const size_t bytes = sendSome(data+done, left);
    done += bytes;
    left -= bytes;
  }

  // send eol
  if( sendSome(g_eol) != Util::tabSize(g_eol) )
    throw SendError("unable to send EOL mark");
}


size_t TextLine::read(uint8_t *data, size_t size, const double timeout)
{
  // sanity check
  if(data==nullptr)
    throw ParameterError("data pointer cannot be NULL");

  const Util::TimeoutClock tout(timeout);       // keep track of remaining time
  bool                     firstRun = true;

  // read-and-check loop
  while(true)
  {
    // check for the EOL within the buffer
    const Buffer::iterator it = findEOL( buf_.begin(), buf_.end() );
    if( it!=buf_.end() )                // got it?
    {
      const size_t len = it - buf_.begin();
      if( size < len )
        throw TooMuchDataInLine();
      assert( buf_.begin()+len==it );
      copy(buf_.begin(), it, data);     // copy to the output buffer
      buf_.erase( buf_.begin(), it+1 ); // erase copyied data, including EOL
      return len;
    }

    // check edge conditions for all but the first run
    if(firstRun==false)
    {
      if( size < buf_.size() )
        throw TooMuchDataInLine();
      if( tout.remaining() == 0 )
        throw Timeout();
    }
    else
      firstRun=false;

    // try reading some more data
    uint8_t tmp[8*1024];
    const size_t got = readSome( tmp, tout.remaining() );
    // copy data to the buffer
    buf_.reserve( buf_.size() + got );
    copy( tmp, tmp+got, back_insert_iterator<Buffer>(buf_) );
  }

  // this code is never reached
  assert(!"we never reach here");
}

}
