#ifndef INCLUDE_IO_CLIENTBUILDER_HPP_FILE
#define INCLUDE_IO_CLIENTBUILDER_HPP_FILE

#include "IO/Builder.hpp"
#include "Net/Address.hpp"
#include "Crypto/BinData.hpp"


namespace IO
{

class ClientBuilder: public Builder
{
public:
  ClientBuilder(const Net::Address& addr, Key key);

private:
  virtual LineComm buildImpl(void);

  Net::Address addr_;
  Key          key_;
};

}

#endif
