#include <vector>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <cinttypes>

#include "Crypto/readKeyFromFile.hpp"
#include "Util/readWholeFile.hpp"

using namespace std;


namespace Crypto
{

IO::LineCommNetwork::Key readKeyFromFile(const std::string& path)
{
  // read file
  vector<uint8_t> keyData = Util::readWholeFile(path);
  IO::LineCommNetwork::Key key;
  // check if sizes do match
  if( keyData.size() != key.size() )
    throw std::runtime_error( (Util::ErrStrm{}<<"key must be exactly "<<key.size()<<" bytes long").str().c_str() );
  // copy to the final destination
  copy( keyData.begin(), keyData.end(), key.data() );
  return key;
}

}
