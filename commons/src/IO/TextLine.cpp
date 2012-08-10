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



TextLine::TextLine(const size_t maxLineLen):
  maxLineLen_(maxLineLen)
{ }


void TextLine::send(const Data& data)
{
  // send data
  size_t done = 0;
  size_t left = data.size();
  while(left>0)
  {
    const size_t bytes = sendSome(data.data()+done, left);
    done += bytes;
    left -= bytes;
  }

  // send eol
  const size_t eolSize = Util::tabSize(g_eol);
  if( sendSome(g_eol, eolSize) != eolSize )
    throw SendError("unable to send EOL mark");
}


void TextLine::read(Data& data, const double timeout)
{
  const Util::TimeoutClock tout(timeout);       // keep track of remaining time
  bool                     firstRun = true;

  // read-and-check loop
  while(true)
  {
    // check for the EOL within the buffer
    // TODO: this find can be optimized by adding offset, to skip parts that have been already checked in the previous iteration(s)
    const Data::iterator it = findEOL( buf_.begin(), buf_.end() );
    if( it!=buf_.end() )                // got it?
    {
      const size_t len = it - buf_.begin();
      assert( buf_.begin()+len==it );
      data.reserve( data.size() + len );                            // prepare place for new data
      copy(buf_.begin(), it, back_insert_iterator<Data>(data) );    // copy to the output buffer
      buf_.erase( buf_.begin(), it+1 );                             // erase copyied data, including EOL
      return;                                                       // ok - we're done here!
    }

    // line too long?
    if( maxLineLen_ <= buf_.size() )
    {
      buf_.clear();                   // next calls should NOT fail - truncte buffer
      throw TooMuchDataInLine();      // report an error
    }

    // check for timeout starting with the 2nd iteration (timeout==0 means one read after all...)
    if( firstRun == false && tout.remaining() == 0 )
      throw Timeout();
    else
      firstRun = false;

    // try reading some more data
    readSome( buf_, tout.remaining() );
  }

  // this code is never reached
  assert(!"we never reach here");
}

}
