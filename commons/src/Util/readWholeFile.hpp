#ifndef INCLUDE_UTIL_READWHOLEFILE_HPP_FILE
#define INCLUDE_UTIL_READWHOLEFILE_HPP_FILE

#include <string>
#include <vector>
#include <stdexcept>
#include <cinttypes>


namespace Util
{

struct FileReadError: public std::runtime_error
{
  FileReadError(const std::string& path, const char *reason);
};

std::vector<uint8_t> readWholeFile(const std::string& path);
}

#endif
