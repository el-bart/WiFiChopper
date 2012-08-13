#ifndef INCLUDE_NET_CHANNELPAIRHELPER_T_HPP_FILE
#define INCLUDE_NET_CHANNELPAIRHELPER_T_HPP_FILE

#include <algorithm>

#include "Util/SocketPairHelper.t.hpp"
#include "Net/Channel.hpp"

namespace
{

struct ChannelPairHelper
{
  ChannelPairHelper(void):
    c1_( {{{1,2,3,4}}, 4242}, std::move(sph_.d1_) ),
    c2_( {{{1,2,3,5}}, 4243}, std::move(sph_.d2_) )
  { }

private:
  SocketPairHelper sph_;

public:
  Net::Channel c1_;
  Net::Channel c2_;
};

}

#endif
