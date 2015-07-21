#include "utils.h"
#include <TROOT.h>
#include <TMath.h>


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

TH1D* util::QuantilesX(TH2*h, Double_t prob, const char * name) {
  return DoQuantiles(h, true, name, prob);
}

//TH1D* util::QuantilesY(TH2*h, Double_t prob, const char * name) {
//return DoQuantiles(h, false, name, prob);
//}

TH1I* util::QuantilesY(TH2*h, Double_t prob, const char * name) {
  // Implementation of quantiles for x or y

  const TAxis *outAxis = 0;
  outAxis = h->GetYaxis();

  // build first name of returned histogram
  TString qname = name;
  if (qname.IsNull() || qname == "_qx" || qname == "_qy") {
    const char * qtype = "qy";
    qname = TString::Format("%s_%s_%3.2f",h->GetName(),qtype, prob);
  }
  // check if the histogram is already existing
  TH1I *h1=0;
  // create the histogram
  h1 = new TH1I(qname, h->GetTitle(), 1, 0, 1);

  // set the bin content
  Int_t firstOutBin = outAxis->GetFirst();
  Int_t lastOutBin = outAxis->GetLast();
  const TArrayD *xbins = outAxis->GetXbins();
  if (xbins->fN == 0)
    h1->SetBins(lastOutBin-firstOutBin+1,outAxis->GetBinLowEdge(firstOutBin),outAxis->GetBinUpEdge(lastOutBin));
  else
    h1->SetBins(lastOutBin-firstOutBin+1,&xbins->fArray[firstOutBin-1]);

  for(int i=1; i<h->GetNbinsY()+1; i++) {
    float total = h->Integral(0, -1, i, i);
    int j = 0;
    float eff = 0;
    if(total != 0) {
      while(eff < prob) {
        j++;
        eff = h->Integral(0, j, i, i) / total;
      }
    }
    h1->SetBinContent(i, j);
  }
  return h1;
}

TH1D* util::DoQuantiles(TH2* h, bool onX, const char * name, Double_t prob) {
  // Implementation of quantiles for x or y

  const TAxis *outAxis = 0;
  const TAxis *inAxis = 0;
  if ( onX )   {
    outAxis = h->GetXaxis();
    inAxis = h->GetYaxis();
  }  else {
    outAxis = h->GetYaxis();
    inAxis = h->GetXaxis();
  }

  // build first name of returned histogram
  TString qname = name;
  if (qname.IsNull() || qname == "_qx" || qname == "_qy") {
    const char * qtype = (onX) ? "qx" : "qy";
    qname = TString::Format("%s_%s_%3.2f",h->GetName(),qtype, prob);
  }
  // check if the histogram is already existing
  TH1D *h1=0;
  // create the histogram
  h1 = new TH1D(qname, h->GetTitle(), 1, 0, 1);

  // set the bin content
  Int_t firstOutBin = outAxis->GetFirst();
  Int_t lastOutBin = outAxis->GetLast();
  const TArrayD *xbins = outAxis->GetXbins();
  if (xbins->fN == 0)
    h1->SetBins(lastOutBin-firstOutBin+1,outAxis->GetBinLowEdge(firstOutBin),outAxis->GetBinUpEdge(lastOutBin));
  else
    h1->SetBins(lastOutBin-firstOutBin+1,&xbins->fArray[firstOutBin-1]);

  // set the bin content of the histogram
  Double_t pp[1];
  pp[0] = prob;

  TH1D * slice = 0;
  for (int ibin = inAxis->GetFirst() ; ibin <= inAxis->GetLast() ; ++ibin) {
    Double_t qq[1];
    // do a projection on the opposite axis
    if(onX) {
      slice = h->ProjectionY("tmp", ibin, ibin, "");
    }
    else {
      slice = h->ProjectionX("tmp", ibin, ibin, "");
    }
    if (!slice) break;
    if (slice->GetSum() == 0) continue;
    slice->GetQuantiles(1,qq,pp);
    h1->SetBinContent(ibin,qq[0]);
    // compute error using normal approximation
    // quantile error  ~  sqrt (q*(1-q)/ *( n * f(xq) ) from Kendall
    // where f(xq) is the p.d.f value at the quantile xqp
    Double_t n = slice->GetEffectiveEntries();
    Double_t f = TMath::Gaus(qq[0], slice->GetStdDev(), kTRUE);
    Double_t error = TMath::Sqrt( prob*(1.-prob)/ ( n * f) );
    h1->SetBinError(ibin, error);
  }
  if (slice) delete slice;
  return h1;
}

