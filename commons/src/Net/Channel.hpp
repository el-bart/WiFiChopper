#ifndef INCLUDE_NET_CHANNEL_HPP_FILE
#define INCLUDE_NET_CHANNEL_HPP_FILE

#include <string>
#include <vector>
#include <algorithm>
#include <cinttypes>

#include "Util/ErrStrm.hpp"
#include "Util/UniqueDescriptor.hpp"
#include "Net/Address.hpp"
#include "Net/Exception.hpp"

namespace Net
{

class Channel
{
public:
  struct InvalidSocket: public Exception
  {
    InvalidSocket(const Address &addr):
      Exception(addr, "invalid socket")
    { }
  };

  struct CallError: public Exception
  {
    CallError(const Address &addr, const char *call);
  };

  struct Timeout: public Exception
  {
    Timeout(const Address &addr):
      Exception(addr, (Util::ErrStrm{}<<"timeout occured while waiting for data from "<<addr).str().c_str() )
    { }
  };


  typedef std::vector<uint8_t> Data;

  Channel(Address addr, Util::UniqueDescriptor sock);

  void write(const Data &in);

  void read(Data &out, double timeout);

private:
  void waitForData(double timeout) const;

  Address                addr_;
  Util::UniqueDescriptor sock_;
};

}

#endif
