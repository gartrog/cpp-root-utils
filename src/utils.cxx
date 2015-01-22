#include "utils.h"


std::string util::replace_first_of(const std::string& in, const std::string& patt, const std::string& rep) {
  size_t pos = in.find_first_of(patt);
  size_t len = patt.size();
  std::string out(in);
  out.replace(pos, len, rep);
  return out;
}


