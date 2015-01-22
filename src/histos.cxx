#include "histos.h"

Histos::Histos() : h1d(), h2d(), cut_type("") {
  TH1::SetDefaultSumw2(true);
}

void Histos::reset() {
  h1d.clear();
  h2d.clear();
  cut_type = "";
}

void Histos::setCut(std::string cut) {
  cut_type = cut;
}

void Histos::addHisto(std::string title, int nbins, float low, float high) {
  std::string name = title.substr(0, title.find_first_of(';'));
  TH1F* h = new TH1F(name.c_str(), title.c_str(), nbins, low, high);
  h->SetDirectory(0);
  h1d[name] = h;
}

void Histos::copyHisto(std::string tplt, std::string name) {
  if(name == "") {
    name = cut_type+"_"+tplt;
  }
  TH1F* h = new TH1F(*(h1d[tplt]));
  h->SetName(name.c_str());
  h->SetTitle(name.c_str());
  h->SetDirectory(0);
  h1d[name] = h;
}

void Histos::fill(std::string name, float value, float weight){
  if(h1d.find(name) == h1d.end()) {
    std::string tplt = name.substr(name.find_last_of('_')+1);
    copyHisto(tplt, name);
  }
  h1d[name]->Fill(value, weight);
}

void Histos::fillCurrent(std::string tplt, float value, float weight) {
  fill(cut_type+"_"+tplt, value, weight);
}

void Histos::addHisto2D(std::string title, int nbinsx, float lowx, float highx, int nbinsy, float lowy, float highy) {
  std::string name = title.substr(0, title.find_first_of(';'));
  TH2F* h = new TH2F(name.c_str(), title.c_str(), nbinsx, lowx, highx, nbinsy, lowy, highy);
  h->SetDirectory(0);
  h2d[name] = h;
}

void Histos::addHisto2DSym(std::string title, int nbins, float low, float high) {
  addHisto2D(title, nbins, low, high, nbins, low, high);
}

void Histos::copyHisto2D(std::string tplt, std::string name) {
  if(name == "") {
    name = cut_type+"_"+tplt;
  }
  TH2F* h = new TH2F(*(h2d[tplt]));
  h->SetName(name.c_str());
  h->SetTitle(name.c_str());
  h->SetDirectory(0);
  h2d[name] = h;
}

void Histos::fill2D(std::string name, float valuex, float valuey, float weight) {
  if(h2d.find(name) == h2d.end()) {
    std::string tplt = name.substr(name.find_last_of('_')+1);
    copyHisto2D(tplt, name);
  }
  h2d[name]->Fill(valuex, valuey, weight);
}

void Histos::fillCurrent2D(std::string tplt, float valuex, float valuey, float weight) {
  fill2D(cut_type+"_"+tplt, valuex, valuey, weight);
}

void Histos::saveHists(TFile* outf) {
  outf->cd();
  // trick to write all histos in alphabetical order, regardless of type of hist.
  std::map<std::string, TH1*> allHists;
  for(auto& p : h1d) {
    allHists.emplace(p.first, p.second);
  }
  for(auto& p : h2d) {
    allHists.emplace(p.first, p.second);
  }
  for(auto& p : allHists) {
    p.second->Write(p.second->GetName());
  }
}


