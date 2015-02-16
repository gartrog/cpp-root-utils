#include "histos.h"

Axis::Axis(int n, float low, float high) : m_bins(n+1) {
  for(int i = 0; i <= n; i++)
    m_bins[i] = low + i* (high-low) / n;
}

Axis::Axis(const std::vector<float>& bins) : m_bins(bins)
{}

const float* Axis::data() const {
  return m_bins.data();
}

int Axis::nbins() const {
  return m_bins.size() - 1;
}




Histos::Histos() : h1d(), h2d(), h3d(), cut_type("") {
  TH1::SetDefaultSumw2(true);
}

void Histos::reset() {
  h1d.clear();
  h2d.clear();
  h3d.clear();
  cut_type = "";
}

void Histos::setCut(std::string cut) {
  cut_type = cut;
}

void Histos::addHisto(std::string title, const Axis& a) {
  std::string name = title.substr(0, title.find_first_of(';'));
  TH1F* h = new TH1F(name.c_str(), title.c_str(), a.nbins(), a.data());
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

void Histos::addHisto2D(std::string title, const Axis& ax, const Axis& ay) {
  std::string name = title.substr(0, title.find_first_of(';'));
  TH2F* h = new TH2F(name.c_str(), title.c_str(), ax.nbins(), ax.data(), ay.nbins(), ay.data());
  h->SetDirectory(0);
  h2d[name] = h;
}

void Histos::addHisto2DSym(std::string title, const Axis& a) {
  addHisto2D(title, a, a);
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

void Histos::addHisto3D(std::string title, const Axis& ax, const Axis& ay, const Axis& az) {
  std::string name = title.substr(0, title.find_first_of(';'));
  TH3F* h = new TH3F(name.c_str(), title.c_str(), ax.nbins(), ax.data(), ay.nbins(), ay.data(), az.nbins(), az.data());
  h->SetDirectory(0);
  h3d[name] = h;
}

void Histos::copyHisto3D(std::string tplt, std::string name) {
  if(name == "") {
    name = cut_type+"_"+tplt;
  }
  TH3F* h = new TH3F(*(h3d[tplt]));
  h->SetName(name.c_str());
  h->SetTitle(name.c_str());
  h->SetDirectory(0);
  h3d[name] = h;
}

void Histos::fill3D(std::string name, float valuex, float valuey, float valuez, float weight) {
  if(h3d.find(name) == h3d.end()) {
    std::string tplt = name.substr(name.find_last_of('_')+1);
    copyHisto3D(tplt, name);
  }
  h3d[name]->Fill(valuex, valuey, valuez, weight);
}

void Histos::fillCurrent3D(std::string tplt, float valuex, float valuey, float valuez, float weight) {
  fill3D(cut_type+"_"+tplt, valuex, valuey, valuez, weight);
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
  for(auto& p : h3d) {
    allHists.emplace(p.first, p.second);
  }
  for(auto& p : allHists) {
    p.second->Write(p.second->GetName());
  }
}


