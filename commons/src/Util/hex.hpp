#ifndef INCLUDE_UTIL_HEX_HPP_FILE
#define INCLUDE_UTIL_HEX_HPP_FILE

#include <string>
#include <vector>
#include <stdexcept>
#include <cstdint>


namespace Util
{

std::string toHex(const std::vector<uint8_t> &in);

std::vector<uint8_t> fromHex(const std::string &in);

}

#endif
