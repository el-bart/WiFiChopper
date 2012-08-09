#include <iterator>
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


size_t TextLine::read(uint8_t *data, size_t size, std::chrono::milliseconds timeout)
{
  // sanity check
  if(data==nullptr)
    throw ParameterError("data pointer cannot be NULL");

  const Util::TimeoutClock<std::chrono::milliseconds> tout(timeout);    // keep track of massing time

  // check if there is EOL within the buffer
  {
    const Buffer::iterator it = findEOL( begin(buf_), end(buf_) );
    if( it!=end(buf_) )
    {
      const size_t len = it - begin(buf_);
      if( size < len )
        throw TooMuchDataInLine();
      copy(begin(buf_), it, data);      // copy to the output buffer
      buf_.erase( begin(buf_), it+1 );  // erase copyied data, including EOL
      return len;
    }
  } // EOL in the buffer?

  // sanity check
  if( buf_.size() > size )
    throw TooMuchDataInLine();
  // copy data and save location where new part is read from
  copy( begin(buf_), end(buf_), data );

  // try obtaining new data
  size_t done = buf_.size();
  size_t left = size - done;
  while(left > 0)
  {
    const size_t got = readSome(data+done, left, tout.remaining() );
    uint8_t *it = findEOL(data+done, data+done+got);
    // found end of line?
    if( it != data+done+got )
    {
      // copy remaining data to the internal buffer
      buf_.clear();
      const uint8_t* from  = it+1;                      // skip this EOL
      const size_t   count = done+got - (from-data);    // 
      //buf_.reserve(
      // TODO
    }
    // no end of line - continue searching...
    done += got;
    left -= got;
  }

  // no EOF found and got end of buffer - write, what has been read, into the internal buffer
  const uint8_t* from  = data + buf_.size();
  const size_t   count = size - buf_.size();
  buf_.reserve(count);
  copy( from, from+count, back_insert_iterator<Buffer>(buf_) );
  // now tell what's wrong...
  throw TooMuchDataInLine();
}

}
