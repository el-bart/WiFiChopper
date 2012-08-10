#ifndef INCLUDE_NET_SOCKETPAIRHELPER_T_HPP_FILE
#define INCLUDE_NET_SOCKETPAIRHELPER_T_HPP_FILE

#include <tut/tut.hpp>
#include <sys/types.h>
#include <sys/socket.h>

#include "Net/Channel.hpp"

namespace
{

struct SocketPairHelper
{
  SocketPairHelper(void):
    // NOTE: this is secure, since UniqueDescriptor's c-tor cannot throw
    c1_( {{{1,2,3,4}}, 4242}, Util::UniqueDescriptor{sp_.sp_[0]} ),
    c2_( {{{1,2,3,5}}, 4243}, Util::UniqueDescriptor{sp_.sp_[1]} )
  { }

private:
  struct Init
  {
    Init(void)
    {
      if( socketpair(AF_LOCAL, SOCK_STREAM, 0, sp_) != 0 )
        tut::fail("unable to create socket pair");
    }

    int sp_[2];
  };

  Init sp_;

public:
  Net::Channel c1_;
  Net::Channel c2_;
};

}

#endif
