#ifndef INCLUDE_UTIL_SOCKETPAIRHELPER_T_HPP_FILE
#define INCLUDE_UTIL_SOCKETPAIRHELPER_T_HPP_FILE

#include <tut/tut.hpp>
#include <sys/types.h>
#include <sys/socket.h>

namespace
{

struct SocketPairHelper
{
  SocketPairHelper(void):
    // NOTE: this is secure, since UniqueDescriptor's c-tor cannot throw
    d1_( Util::UniqueDescriptor{sp_.sp_[0]} ),
    d2_( Util::UniqueDescriptor{sp_.sp_[1]} )
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
  Util::UniqueDescriptor d1_;
  Util::UniqueDescriptor d2_;
};

}

#endif
