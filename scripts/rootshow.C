#include <TFile.h>
#include <TCanvas.h>
#include <iostream>

void rootshow(const TString& fname, const TString cname = "c") {
  TFile* f = TFile::Open(fname);
  if(!f) {
    std::cerr << "ERROR: file " << fname << " does not exist !" << std::endl;
    return;
  }
  TCanvas* c = reinterpret_cast<TCanvas*>(f->Get(cname));
  if(!c) {
    std::cerr << "ERROR: canvas " << cname << " does not exist in file " << fname << " !" << std::endl;
    return;
  }
  c->Draw();
}
