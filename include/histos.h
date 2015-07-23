#ifndef libhistos_h
#define libhistos_h

#include <unordered_map>
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
    std::vector<std::string> m_labels;

  public:
    Axis(int n, float low, float high);
    Axis(const std::vector<float>& bins);
    Axis(const std::vector<std::string>& bins);
    Axis() = delete;
    Axis(const Axis&) = default;
    Axis(Axis&&) = default;

    const float* data() const;
    int nbins() const;
    bool hasLabels() const;
    const std::vector<std::string>& labels() const;

    static void setLabels(TAxis* axis, const Axis& a);
};

class Histos
{
  private:
    std::unordered_map<std::string, TH1F*> h1d;
    std::unordered_map<std::string, TH2F*> h2d;
    std::unordered_map<std::string, TH3F*> h3d;
    std::string cut_type;

  public:
    Histos();
    ~Histos() = default;
    void reset();
    void setCut(const std::string& cut);
    std::string getCut() const;

    void addHisto(const std::string& title, const Axis& a);
    void copyHisto(const std::string& tplt, std::string name = "");
    void fill(const std::string& name, float value, float weight = 1);
    void fillCurrent(const std::string& tplt, float value, float weight = 1);
    void fill(const std::string& name, const std::string& binname, float weight = 1);
    void fillCurrent(const std::string& tplt, const std::string& binname, float weight = 1);

    void addHisto2D(const std::string& title, const Axis& ax, const Axis& ay);
    void addHisto2DSym(const std::string& title, const Axis& a);
    void copyHisto2D(const std::string& tplt, std::string name = "");
    void fill2D(const std::string& name, float valuex, float valuey, float weight = 1);
    void fillCurrent2D(const std::string& tplt, float valuex, float valuey, float weight = 1);
    void fill2D(const std::string& name, const std::string& binnamex, float valuey, float weight = 1);
    void fillCurrent2D(const std::string& tplt, const std::string& binnamex, float valuey, float weight = 1);
    void fill2D(const std::string& name, float valuex, const std::string& binnamey, float weight = 1);
    void fillCurrent2D(const std::string& tplt, float valuex, const std::string& binnamey, float weight = 1);
    void fill2D(const std::string& name, const std::string& binnamex, const std::string& binnamey,
                float weight = 1);
    void fillCurrent2D(const std::string& tplt, const std::string& binnamex, const std::string& binnamey,
                       float weight = 1);

    void addHisto3D(const std::string& title, const Axis& ax, const Axis& ay, const Axis& az);
    void copyHisto3D(const std::string& tplt, std::string name = "");
    void fill3D(const std::string& name, float valuex, float valuey, float valuez, float weight = 1);
    void fillCurrent3D(const std::string& tplt, float valuex, float valuey, float valuez, float weight = 1);
    // TODO fill with bin labels

    void saveHists(TFile* outf);

  private:
    void copyIfNotExists(const std::string& name);
    void copyIfNotExists2D(const std::string& name);
};

#endif
