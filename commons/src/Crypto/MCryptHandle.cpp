#include <cstring>
#include <cassert>

#include "Crypto/MCryptHandle.hpp"


namespace Crypto
{

MCryptHandle::MCryptHandle(const std::string& algo, const std::string& mode):
  algo_( copyStr(algo) ),
  mode_( copyStr(mode) ),
  td_( mcrypt_module_open(algo_.get(), nullptr, mode_.get(), nullptr) )
{
  if(td_==MCRYPT_FAILED)
    throw std::runtime_error("mcrypt_module_open(): failed to open cypher");
}


MCryptHandle::~MCryptHandle(void)
{
  if(td_!=MCRYPT_FAILED)
    mcrypt_module_close(td_);
}


MCryptHandle::CString MCryptHandle::copyStr(const std::string &str)
{
  CString tmp(new char[str.length()+1]);
  strcpy(tmp.get(), str.c_str());
  assert( strlen(tmp.get()) == str.length() );
  return std::move(tmp);
}

}
