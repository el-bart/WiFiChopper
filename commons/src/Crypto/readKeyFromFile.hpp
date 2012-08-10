#ifndef INCLUDE_CRYPTO_READKEYFROMFILE_HPP_FILE
#define INCLUDE_CRYPTO_READKEYFROMFILE_HPP_FILE

#include <string>

#include "IO/LineCommNetwork.hpp"


namespace Crypto
{
IO::LineCommNetwork::Key readKeyFromFile(const std::string& path);
}

#endif
