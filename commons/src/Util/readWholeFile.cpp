#include <fstream>
#include <cassert>

#include "Util/readWholeFile.hpp"
#include "Util/ErrStrm.hpp"

using namespace std;

namespace Util
{

FileReadError::FileReadError(const std::string& path, const char* reason):
  runtime_error( (ErrStrm{}<<"error while reading/opening file '"<<path<<"': "<<reason).str().c_str() )
{ }


std::vector<uint8_t> readWholeFile(const std::string& path)
{
  // open
  ifstream in( path.c_str() );
  if( !in.is_open() )
    throw FileReadError(path, "cannot open file");
  // read
  try
  {
    return vector<uint8_t>( istreambuf_iterator<char>{in}, istreambuf_iterator<char>{} );
  }
  catch(const std::exception& ex)
  {
    throw FileReadError( path, ex.what() );
  }
  assert(!"this code is never reached");
}

}
