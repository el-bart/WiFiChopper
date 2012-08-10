#ifndef INCLUDE_NET_RESOLVER_HPP_FILE
#define INCLUDE_NET_RESOLVER_HPP_FILE

#include <string>
#include <vector>

#include "Util/ErrStrm.hpp"
#include "Net/Address.hpp"
#include "Net/Exception.hpp"


namespace Net
{

class Resolver
{
private:
  typedef std::vector<IPv4> IPs;

public:
  struct CannotResolve: public Exception
  {
    CannotResolve(const std::string& host, const char* detail):
      Exception( (Util::ErrStrm{}<<"cannot resolve host '"<<host<<"': "<<detail).str() )
    { }
  };

  typedef IPs::size_type      size_type;
  typedef IPs::value_type     value_type;
  typedef IPs::const_iterator const_iterator;

  explicit Resolver(const std::string& host);

  IPs::size_type size(void) const { return ips_.size(); }
  const IPv4& operator[](size_t n) const { return ips_[n]; }

  const_iterator begin(void) const { return ips_.begin(); }
  const_iterator end(void) const { return ips_.end(); }

private:
  IPs ips_;
};

}

#endif
