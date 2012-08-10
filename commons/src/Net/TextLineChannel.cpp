#include <iterator>
#include <algorithm>

#include "Net/TextLineChannel.hpp"

using namespace std;


namespace Net
{

TextLineChannel::TextLineChannel(Channel c):
  c_( std::move(c) )
{ }


size_t TextLineChannel::sendSome(const Data &data, const size_t skip)
{
  if(skip==0)
  {
    // ok - wirte all!
    c_.write(data);
    return data.size();
  }

  // ehh - write just the remaining part...
  const Data tmp( data.begin()+skip, data.end() );
  c_.write(tmp);
  return tmp.size();
}


size_t TextLineChannel::readSome(Data& data, double timeout)
{
  const size_t size = data.size();
  c_.read(data, timeout);
  return data.size() - size;
}

}
