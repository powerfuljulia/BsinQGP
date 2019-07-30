#include <RooHist.h>
#include <TSystem.h>
#include <sstream>
//#include "ModelConfig.h"
#include <fstream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <TStyle.h>
#include <TAxis.h>
#include <TLatex.h>
#include <TPaveText.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <TTree.h>
#include <TLegend.h>
#include <TString.h>
#include <TCanvas.h>
#include <TNtupleD.h>
#include <TChain.h>
#include <TH1D.h>
#include <TLorentzVector.h>
#include <RooRealVar.h>
#include <RooProduct.h>
#include <RooConstVar.h>
#include <RooDataSet.h>
#include <RooGaussian.h>
#include <RooBifurGauss.h>
#include <RooChebychev.h>
#include <RooBernstein.h>
#include <RooExponential.h>
#include <RooWorkspace.h>
#include <RooAddPdf.h>
#include <RooGenericPdf.h>
#include <RooCBShape.h>
#include <RooArgusBG.h>
#include <TGraphAsymmErrors.h>
#include <TEfficiency.h>
#include <RooMCStudy.h>
#include <RooPlot.h>
#include <RooPlotable.h>
#include <RooThresholdCategory.h>
#include <Roo1DTable.h>
#include "TMath.h"
#include "TVectorD.h"
#include <TLegend.h>
#include "TF1.h"
#include "TPaveStats.h"

#include "/home/t3cms/julia/LSTORE/CMSSW_7_5_8_patch5/src/UserCode/B_production_x_sec_13_TeV/interface/plotDressing.h"

#define BS_MASS 5.36677
#define KSTAR_MASS 0.89594
#define B0_MASS 5.27958
#define PHI_MASS 1.019455
#define KAON_MASS 0.493677
#define BP_MASS 5.27926

TString channel_to_ntuple_name(int channel)
{
  //returns a TString with the ntuple name corresponding to the channel. It can be used to find the data on each channel saved in a file. or to write the name of a directory                                                                                                                               
  TString ntuple_name = "";
  
  switch(channel){
  default:
  case 1:
    ntuple_name="ntKp";
    break;
  case 2:
    ntuple_name="ntKstar";
    break;
  case 3:
   ntuple_name="ntKs";
   break;
  case 4:
    ntuple_name="ntphi";
    break;
  case 5:
   ntuple_name="ntmix";
   break;
  case 6:
    ntuple_name="ntlambda";
    break;
 }
  return ntuple_name;
}

TString channel_to_xaxis_title(int channel)
{
  TString xaxis_title = "";
  
  switch (channel) {
  default:
  case 1:
    xaxis_title = "M_{J/#psi K^{#pm}} [GeV]";
    break;
  case 2:
    xaxis_title = "M_{J/#psi K^{#pm}#pi^{#mp}} [GeV]";
    break;
  case 3:
    xaxis_title = "M_{J/#psi K^{0}_{S}} [GeV]";
    break;
  case 4:
    xaxis_title = "M_{J/#psi K^{#pm}K^{#mp}} [GeV]";
    break;
  case 5:
    xaxis_title = "M_{J/#psi #pi^{#pm}#pi^{#mp}} [GeV]";
    break;
  case 6:
    xaxis_title = "M_{J/#psi #Lambda} [GeV]";
    break;
  }
  return xaxis_title;
}


void read_data(RooWorkspace& w, TString filename, int channel)
{

  TFile* f = new TFile(filename);
  std::cout<<"new file"<<std::endl;
  TNtupleD* _nt = (TNtupleD*)f->Get(channel_to_ntuple_name(channel));
  std::cout<<"new ntuple"<<std::endl;
 
  RooArgSet arg_list(*(w.var("Bmass")) , *(w.var("Bpt")) , *(w.var("By")));
  std::cout<<"new argset"<<std::endl;


  RooDataSet* data = new RooDataSet("data","data",_nt,arg_list);
  std::cout<<"new data_Set"<<std::endl;

  w.import(*data);
  std::cout<<"import"<<std::endl;

}


void set_up_workspace_variables(RooWorkspace& w)
{
  double mass_min, mass_max;
  double pt_min, pt_max;
  double y_min, y_max;

  pt_min=0;
  pt_max=300;
  
  y_min=-3;
  y_max=3;

  mass_min=5.14;
  mass_max=5.6;

  RooRealVar Bmass("Bmass","Bmass",mass_min,mass_max);
  RooRealVar Bpt("Bpt","Bpt",pt_min,pt_max);
  RooRealVar By("By","By",y_min,y_max);

  w.import(Bmass);
  w.import(Bpt);
  w.import(By);
}



