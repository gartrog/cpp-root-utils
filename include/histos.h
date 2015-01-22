#ifndef libhistos_h
#define libhistos_h

#include <map>
#include <string>

#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>

class Histos
{
  private:
    std::map<std::string, TH1F*> h1d;
    std::map<std::string, TH2F*> h2d;
    std::string cut_type;

  public:
    Histos();
    ~Histos() = default;
    void reset();
    void setCut(std::string cut);

    void addHisto(std::string title, int nbins, float low, float high);
    void copyHisto(std::string tplt, std::string name = "");
    void fill(std::string name, float value, float weight = 1);
    void fillCurrent(std::string tplt, float value, float weight = 1);
    void addHisto2D(std::string title, int nbinsx, float lowx, float highx, int nbinsy, float lowy, float highy);
    void addHisto2DSym(std::string title, int nbins, float low, float high);
    void copyHisto2D(std::string tplt, std::string name = "");
    void fill2D(std::string name, float valuex, float valuey, float weight = 1);
    void fillCurrent2D(std::string tplt, float valuex, float valuey, float weight = 1);
    void saveHists(TFile* outf);

};

#endif
