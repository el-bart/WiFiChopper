#include <string>
#include <algorithm>

#include "IO/LineComm.hpp"
#include "Crypto/Aes256.hpp"
#include "Net/TextLineChannel.hpp"
#include "Util/hex.hpp"

using namespace std;


namespace IO
{

LineComm::Key::Key(void):
  Crypto::BinData( Crypto::Aes256::keySize() )
{ }


LineComm::LineComm(Net::Channel chn, Key key):
  crypt_( new Crypto::Aes256( std::move(key) ) ),
  comm_( new Net::TextLineChannel( std::move(chn) ) )
{ }


void LineComm::send(const std::string& line)
{
  // encrypt input data
  Crypto::Algo::Data data( line.begin(), line.end() );
  crypt_->encrypt(data);
  // write encoded text as string
  string encStr = Util::toHex(data);
  // send encoded line
  comm_->send( std::move(encStr) );
}


std::string LineComm::read(const double timeout)
{
  // read encrypted message
  string encStr = comm_->read(timeout);
  // conver it back to binary
  Crypto::Algo::Data enc = Util::fromHex( std::move(encStr) );
  // decode text
  crypt_->decrypt(enc);
  // return final message
  return string( enc.begin(), enc.end() );
}

}
