#ifndef libhistos_h
#define libhistos_h

#include <map>
#include <string>
#include <vector>

#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TFile.h>

class Axis
{
  private:
    std::vector<float> m_bins;

  public:
    Axis(int n, float low, float high);
    Axis(const std::vector<float>& bins);
    Axis() = delete;
    Axis(const Axis&) = default;
    Axis(Axis&&) = default;

    const float* data() const;
    int nbins() const;
};

class Histos
{
  private:
    std::map<std::string, TH1F*> h1d;
    std::map<std::string, TH2F*> h2d;
    std::map<std::string, TH3F*> h3d;
    std::string cut_type;

  public:
    Histos();
    ~Histos() = default;
    void reset();
    void setCut(std::string cut);

    void addHisto(std::string title, const Axis& a);
    void copyHisto(std::string tplt, std::string name = "");
    void fill(std::string name, float value, float weight = 1);
    void fillCurrent(std::string tplt, float value, float weight = 1);

    void addHisto2D(std::string title, const Axis& ax, const Axis& ay);
    void addHisto2DSym(std::string title, const Axis& a);
    void copyHisto2D(std::string tplt, std::string name = "");
    void fill2D(std::string name, float valuex, float valuey, float weight = 1);
    void fillCurrent2D(std::string tplt, float valuex, float valuey, float weight = 1);

    void addHisto3D(std::string title, const Axis& ax, const Axis& ay, const Axis& az);
    void copyHisto3D(std::string tplt, std::string name = "");
    void fill3D(std::string name, float valuex, float valuey, float valuez, float weight = 1);
    void fillCurrent3D(std::string tplt, float valuex, float valuey, float valuez, float weight = 1);

    void saveHists(TFile* outf);

};

#endif
