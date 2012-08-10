#ifndef INCLUDE_NET_TEXTLINECHANNEL_HPP_FILE
#define INCLUDE_NET_TEXTLINECHANNEL_HPP_FILE

#include "IO/TextLine.hpp"
#include "Net/Channel.hpp"

namespace Net
{

class TextLineChannel: public IO::TextLine
{
public:
  explicit TextLineChannel(Channel c);

private:
  virtual size_t sendSome(const Data &data, size_t skip);
  virtual size_t readSome(Data& data, double timeout);

  Channel c_;
};

}

#endif
