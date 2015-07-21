#ifndef utils_h
#define utils_h

#include <string>
#include <TH2.h>
#include <TH1D.h>
#include <TH1I.h>

namespace util {
  std::string replace_first_of(const std::string& in, const std::string& patt, const std::string& rep);
  std::string replace_last_of(const std::string& in, const std::string& patt, const std::string& rep);
  TH1D* QuantilesX(TH2*h, Double_t prob, const char * name);
  TH1I* QuantilesY(TH2*h, Double_t prob, const char * name);
  TH1D* DoQuantiles(TH2* h, bool onX, const char * name, Double_t prob);
};

#endif
