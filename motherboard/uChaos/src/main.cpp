#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "Crypto/Aes256.hpp"

using namespace std;

Crypto::BinData makeKIV(void)
{
  Crypto::BinData tmp( Crypto::Aes256::keySize() );
  for(size_t i=0; i<tmp.size(); ++i)
    tmp.data()[i]=i;
  return std::move(tmp);
}


int main(int argc, char **argv)
{
  std::cout << argv[0] << ": one big TODO... :)" << std::endl;
  Crypto::Aes256 enc( makeKIV(), makeKIV() );
  Crypto::Aes256 dec( makeKIV(), makeKIV() );
  Crypto::Aes256::Data data;
  for(const char *it="ala ma kota"; *it!=0; ++it)
    data.push_back(*it);

  enc.encrypt(data);
  dec.decrypt(data);
  cout << "got : |";
  copy( data.begin(), data.end(), ostreambuf_iterator<char>(cout) );
  cout << "|\n";
  return 0;
}
