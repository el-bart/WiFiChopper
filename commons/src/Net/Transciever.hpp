#ifndef INCLUDE_NET_TRANSCIEVER_HPP_FILE
#define INCLUDE_NET_TRANSCIEVER_HPP_FILE

#include <chrono>
#include <cinttypes>

#include "Util/ErrStrm.hpp"
#include "Util/UniqueDescriptor.hpp"
#include "Net/Exception.hpp"


namespace Net
{

class Transciever
{
public:
  struct IPv4
  {
    uint8_t addr_[4];
  };

  struct Address
  {
    IPv4     ip_;
    uint16_t port_;
  };


  struct InvalidDescriptor: public Exception
  {
    InvalidDescriptor(void):
      Exception("invalid descriptor")
    { }
  };

  struct ErrorIO: public Exception
  {
    ErrorIO(const Address& addr, std::string details):
      Exception( (Util::ErrStrm{} << "I/O error while talking to " << addr << " - " << std::move(details)).str() )
    { }
  };

  struct ConnectionError: public Exception
  {
    ConnectionError(const Address& addr, std::string details):
      Exception( (Util::ErrStrm{} << addr << " - " << std::move(details)).str() )
    { }
  };

  struct ConnectionTimeout: public Exception
  {
    ConnectionTimeout(const Address& addr, std::string details):
      Exception( (Util::ErrStrm{} << "timeout while connecting to " << addr).str() )
    { }
  };


  explicit Transciever(Util::UniqueDescriptor&& rep);

  template<uint8_t N>
  void send(const uint8_t(&buf)[N])
  {
    send(buf, N);
  }

  void send(const uint8_t *buf, uint8_t size);
  void read(const uint8_t *buf, uint8_t size, std::chrono::milliseconds timeout);

  void swap(Transciever& other)
  {
    std::swap(rep_, other.rep_);
  }

private:
  Util::UniqueDescriptor rep_;
};



template<typename S>
S&& operator<<(S&& strm, const Transciever::IPv4& ip)
{
  bool first=true;
  for(auto f: ip.addr_)
  {
    if(first)
      first=false;
    else
      strm << '.';
    strm << static_cast<int>(f);
  }
  return strm;
}

template<typename S>
S&& operator<<(S&& strm, const Transciever::Address& addr)
{
  strm << addr.ip_ << ':' << static_cast<int>(addr.port_);
  return strm;
}

}

#endif
