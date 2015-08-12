#include "histos.h"
#include <map>

Axis::Axis(int n, float low, float high) : m_bins(n+1), m_labels() {
  for(int i = 0; i <= n; i++)
    m_bins[i] = low + i* (high-low) / n;
}

Axis::Axis(const std::vector<float>& bins) : m_bins(bins), m_labels()
{}

Axis::Axis(const std::vector<std::string>& bins) : m_bins(bins.size()+1), m_labels(bins)
{
  for(unsigned int i = 0; i < bins.size()+1; i++) {
    m_bins[i] = i;
  }
}

const float* Axis::data() const {
  return m_bins.data();
}

int Axis::nbins() const {
  return m_bins.size() - 1;
}

bool Axis::hasLabels() const {
  return (m_labels.size()>0);
}

const std::vector<std::string>& Axis::labels() const {
  return m_labels;
}

void Axis::setLabels(TAxis* axis, const Axis& a) {
  if(a.hasLabels()) {
    const std::vector<std::string>& l = a.labels();
    for(unsigned int i = 0; i < l.size(); i++) {
      axis->SetBinLabel(i+1, l[i].c_str());
    }
  }
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

void Histos::setCut(const std::string& cut) {
  cut_type = cut;
}

std::string Histos::getCut() const {
  return cut_type;
}

void Histos::addHisto(const std::string& title, const Axis& a) {
  std::string name = title.substr(0, title.find_first_of(';'));
  TH1F* h = new TH1F(name.c_str(), title.c_str(), a.nbins(), a.data());
  h->SetDirectory(0);
  Axis::setLabels(h->GetXaxis(), a);
  h1d[name] = h;
}

void Histos::copyHisto(const std::string& tplt, std::string name) {
  if(name == "") {
    name = cut_type+"_"+tplt;
  }
  TH1F* h = new TH1F(*(h1d[tplt]));
  h->SetName(name.c_str());
  h->SetTitle(name.c_str());
  h->SetDirectory(0);
  h1d[name] = h;
}

void Histos::copyIfNotExists(const std::string& name) {
  if(h1d.find(name) == h1d.end()) {
    std::string tplt = name.substr(name.find_last_of('_')+1);
    copyHisto(tplt, name);
  }
}

void Histos::fill(const std::string& name, float value, float weight){
  copyIfNotExists(name);
  h1d[name]->Fill(value, weight);
}

void Histos::fillCurrent(const std::string& tplt, float value, float weight) {
  fill(cut_type+"_"+tplt, value, weight);
}

void Histos::fill(const std::string& name, const std::string& binname, float weight){
  copyIfNotExists(name);
  h1d[name]->Fill(binname.c_str(), weight);
}

void Histos::fillCurrent(const std::string& tplt, const std::string& binname, float weight) {
  fill(cut_type+"_"+tplt, binname, weight);
}

void Histos::addHisto2D(const std::string& title, const Axis& ax, const Axis& ay) {
  std::string name = title.substr(0, title.find_first_of(';'));
  TH2F* h = new TH2F(name.c_str(), title.c_str(), ax.nbins(), ax.data(), ay.nbins(), ay.data());
  h->SetDirectory(0);
  Axis::setLabels(h->GetXaxis(), ax);
  Axis::setLabels(h->GetYaxis(), ay);
  h2d[name] = h;
}

void Histos::addHisto2DSym(const std::string& title, const Axis& a) {
  addHisto2D(title, a, a);
}

void Histos::copyHisto2D(const std::string& tplt, std::string name) {
  if(name == "") {
    name = cut_type+"_"+tplt;
  }
  TH2F* h = new TH2F(*(h2d[tplt]));
  h->SetName(name.c_str());
  h->SetTitle(name.c_str());
  h->SetDirectory(0);
  h2d[name] = h;
}

void Histos::copyIfNotExists2D(const std::string& name) {
  if(h2d.find(name) == h2d.end()) {
    std::string tplt = name.substr(name.find_last_of('_')+1);
    copyHisto2D(tplt, name);
  }
}

void Histos::fill2D(const std::string& name, float valuex, float valuey, float weight) {
  copyIfNotExists2D(name);
  h2d[name]->Fill(valuex, valuey, weight);
}

void Histos::fillCurrent2D(const std::string& tplt, float valuex, float valuey, float weight) {
  fill2D(cut_type+"_"+tplt, valuex, valuey, weight);
}

void Histos::fill2D(const std::string& name, const std::string& binnamex, float valuey, float weight) {
  copyIfNotExists2D(name);
  h2d[name]->Fill(binnamex.c_str(), valuey, weight);
}

void Histos::fillCurrent2D(const std::string& tplt, const std::string& binnamex, float valuey, float weight) {
  fill2D(cut_type+"_"+tplt, binnamex, valuey, weight);
}

void Histos::fill2D(const std::string& name, float valuex, const std::string& binnamey, float weight) {
  copyIfNotExists2D(name);
  h2d[name]->Fill(valuex, binnamey.c_str(), weight);
}

void Histos::fillCurrent2D(const std::string& tplt, float valuex, const std::string& binnamey, float weight) {
  fill2D(cut_type+"_"+tplt, valuex, binnamey, weight);
}

void Histos::fill2D(const std::string& name, const std::string& binnamex,
                    const std::string& binnamey, float weight) {
  copyIfNotExists2D(name);
  h2d[name]->Fill(binnamex.c_str(), binnamey.c_str(), weight);
}

void Histos::fillCurrent2D(const std::string& tplt, const std::string& binnamex,
                           const std::string& binnamey, float weight) {
  fill2D(cut_type+"_"+tplt, binnamex, binnamey, weight);
}

void Histos::addHisto3D(const std::string& title, const Axis& ax, const Axis& ay, const Axis& az) {
  std::string name = title.substr(0, title.find_first_of(';'));
  TH3F* h = new TH3F(name.c_str(), title.c_str(), ax.nbins(), ax.data(),
                     ay.nbins(), ay.data(), az.nbins(), az.data());
  h->SetDirectory(0);
  h3d[name] = h;
}

void Histos::copyHisto3D(const std::string& tplt, std::string name) {
  if(name == "") {
    name = cut_type+"_"+tplt;
  }
  TH3F* h = new TH3F(*(h3d[tplt]));
  h->SetName(name.c_str());
  h->SetTitle(name.c_str());
  h->SetDirectory(0);
  h3d[name] = h;
}

void Histos::fill3D(const std::string& name, float valuex, float valuey, float valuez, float weight) {
  if(h3d.find(name) == h3d.end()) {
    std::string tplt = name.substr(name.find_last_of('_')+1);
    copyHisto3D(tplt, name);
  }
  h3d[name]->Fill(valuex, valuey, valuez, weight);
}

void Histos::fillCurrent3D(const std::string& tplt, float valuex, float valuey, float valuez, float weight) {
  fill3D(cut_type+"_"+tplt, valuex, valuey, valuez, weight);
}

void Histos::saveHists(TFile* outf, bool removeEmptyHists) {
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
    if(!removeEmptyHists || (p.second->GetEntries()>0)) {
      p.second->Write(p.second->GetName());
    }
  }
}


