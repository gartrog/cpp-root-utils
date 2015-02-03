#include "utils.h"


std::string util::replace_first_of(const std::string& in, const std::string& patt, const std::string& rep) {
  size_t pos = in.find(patt);
  size_t len = patt.size();
  std::string out(in);
  if(pos != std::string::npos) {
    out.replace(pos, len, rep);
  }
  return out;
}

std::string util::replace_last_of(const std::string& in, const std::string& patt, const std::string& rep) {
  size_t pos = in.rfind(patt);
  size_t len = patt.size();
  std::string out(in);
  if(pos != std::string::npos) {
    out.replace(pos, len, rep);
  }
  return out;
}

