#ifndef INCLUDE_IO_SERVERBUILDER_HPP_FILE
#define INCLUDE_IO_SERVERBUILDER_HPP_FILE

#include "IO/Builder.hpp"
#include "Net/Server.hpp"
#include "Net/Address.hpp"
#include "Crypto/BinData.hpp"


namespace IO
{

class ServerBuilder: public Builder
{
public:
  ServerBuilder(const Net::Address& addr, Key key);

private:
  virtual LineCommPtr buildImpl(void);

  Net::Server s_;
  Key         key_;
};

}

#endif
