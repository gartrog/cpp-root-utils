#ifndef utils_h
#define utils_h

#include <string>

namespace util {
  std::string replace_first_of(const std::string& in, const std::string& patt, const std::string& rep);
  std::string replace_last_of(const std::string& in, const std::string& patt, const std::string& rep);
};

#endif
