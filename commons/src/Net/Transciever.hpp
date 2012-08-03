#ifndef INCLUDE_NET_TRANSCIEVER_HPP_FILE
#define INCLUDE_NET_TRANSCIEVER_HPP_FILE

#include <cinttypes>

#include "Util/UniqueDescriptor.hpp"


namespace Net
{

class Transciever
{
public:
  struct IPv4
  {
    uint8_t addr_[4];
  };

  explicit Transciever(Util::UniqueDescriptor&& rep);

  template<uint8_t N>
  void send(const uint8_t(&buf)[N])
  {
    send(buf, N);
  }

  void send(const uint8_t *buf, uint8_t size);
  void read(const uint8_t *buf, uint8_t size);

  void swap(Transciever& other)
  {
    std::swap(rep_, other.rep_);
  }

private:
  Util::UniqueDescriptor rep_;
};

}

#endif
