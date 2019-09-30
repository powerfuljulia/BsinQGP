
#include <sstream>
#include <vector>
#include <cmath>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TNtupleD.h>
#include <TH1D.h>
#include <TLorentzVector.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TMath.h>

#include "/home/t3cms/julia/LSTORE/CMSSW_7_5_8_patch5/src/UserCode/Bs_analysis/interface/new_loop.h"
#include "/home/t3cms/julia/LSTORE/CMSSW_7_5_8_patch5/src/UserCode/Bs_analysis/interface/auxfunctions.h"
bool test=false;
bool optimized=true;


using namespace std;
//pp:csyst=1, PbPb:csyst=2
void data_selection(TString fin1,TString data_selection_output_file, int csyst, int mc, int channel);


int main(int argc, char *argv[]){
  
  std::string collisionsystem="";
  int mc=0;
  int channel=4;
  
  for(int i=1 ; i<argc ; ++i){
    std::string argument = argv[i];
    std::stringstream convert;
    
    if(argument == "--colsyst"){
      convert << argv[++i];
      convert >> collisionsystem;
    }
    
    if(argument == "--mc"){
      convert << argv[++i];
      convert >> mc;
    }
    
    if(argument == "--channel"){
      convert << argv[++i];
      convert >> channel;
    }
    
  }
  
  
  TString input_file_pp = "/lstore/cms/julia/pp_files/ntB_EvtBase_20160420_BfinderData_pp_20160419_bPt0jpsiPt0tkPt0p5.root";
  TString output_file_pp= "/exper-sw/cmst3/cmssw/users/julia/CMSSW_7_5_8_patch5/src/UserCode/Bs_analysis/selected_data/selected_data_" + channel_to_ntuple_name(channel) + "_pp.root";
  //TString input_file_PbPb = "/lstore/cms/julia/crab_Bfinder_20181220_HIDoubleMuon_HIRun2018A_PromptReco_v1v2_1031_NoJSON_skimhltBsize_ntKp.root";
  TString output_file_PbPb= "/home/t3cms/julia/LSTORE/CMSSW_7_5_8_patch5/src/UserCode/Bs_analysis/flat_trees/selected_data_" + channel_to_ntuple_name(channel) + "_PbPb_2018_prefilter.root";
  TString output_file_pp_mc= "/exper-sw/cmst3/cmssw/users/julia/CMSSW_7_5_8_patch5/src/UserCode/Bs_analysis/mc_for_central_code/selected_mc_ntphi_pp.root";
  TString input_file_pp_mc = "/lstore/cms/julia/pp_files/loop_Bs0_pthat5_bstojpsiphi_pp.root";
  TString output_file_PbPb_mc= "/home/t3cms/julia/LSTORE/CMSSW_7_5_8_patch5/src/UserCode/Bs_analysis/flat_trees/selected_mc_" + channel_to_ntuple_name(channel) + "_PbPb_2018_prefilter.root";
//  TString input_file_PbPb_mc = "/lstore/cms/julia/crab_Bfinder_20190221_Pythia8_BuToJpsiK_Bpt0p0_1032_NoJSON_pthatweight_hardcut_v2.root";
  std::cout<<"channel= "<<channel<<std::endl;
  TString input_file_PbPb;
  TString input_file_PbPb_mc;
  switch (channel) {
      default: 
        input_file_PbPb = "/lstore/cms/julia/most_updated_samples/Data_Bs_PbPb_TMVA_BDT_PbPb.root";
        input_file_PbPb_mc  = "/lstore/cms/julia/most_updated_samples/MC_Bs_PbPb_TMVA_BDT_PbPb.root";
      case 1:
        input_file_PbPb = "/lstore/cms/julia/most_updated_samples/crab_Bfinder_20190513_HIDoubleMuon__PsiPeri__HIRun2018A_04Apr2019_v1_1033p1_GoldenJSON_skimhltBsize_ntKp_BDT.root";
        input_file_PbPb_mc = "/lstore/cms/julia/most_updated_samples/crab_Bfinder_20190624_Hydjet_Pythia8_Official_BuToJpsiK_1033p1_pt3tkpt0p7dls2_allpthat_pthatweight_BDT.root";
        break;
      case 4:
        input_file_PbPb = "/lstore/cms/julia/most_updated_samples/Data_Bs_PbPb_TMVA_BDT_PbPb.root ";
        input_file_PbPb_mc  = "/lstore/cms/julia/most_updated_samples/MC_Bs_PbPb_TMVA_BDT_PbPb.root";
        break;
  }  


  if(collisionsystem=="pp" && mc==0){
    std::cout<<"Selecting data from pp collisions"<<std::endl;
    data_selection(input_file_pp,output_file_pp, 1, 0, channel);
  }
  if(collisionsystem=="PbPb" && mc==0){
    std::cout<<"Selecting data from PbPb collisions"<<std::endl;
    data_selection(input_file_PbPb,output_file_PbPb, 2, 0, channel);
  }
  if(collisionsystem=="pp" && mc==1){
    std::cout<<"Selecting mc from pp collisions"<<std::endl;
    data_selection(input_file_pp_mc,output_file_pp_mc, 1, 1, channel);
  }
  if(collisionsystem=="PbPb" && mc==1){
    std::cout<<"Selecting mc from PbPb collisions"<<std::endl;
    data_selection(input_file_PbPb_mc,output_file_PbPb_mc, 2, 1, channel);
  }
  
}

void data_selection(TString fin1, TString data_selection_output_file, int csyst, int mc, int channel){
  
  std::cout << "Reading Input TTree " << std::endl;
  TFile *f = new TFile(fin1);
  std::cout << "Going to Bfnder directory " << std::endl;
  std::cout << "FILEEEE: " <<fin1<<std::endl;
//  TDirectory *cdtof =f->GetDirectory("/lstore/cms/julia/after_training/Data_Bs_PbPb_TMVA_BDT_PbPb.root:/Bfinder");
  TDirectory *cdtof =f->GetDirectory("Bfinder");
  cdtof->ls();
  cdtof->cd();    // make the "tof" directory the current directory
  
  TTree *t1 = (TTree*)cdtof->Get(channel_to_ntuple_name(channel));  
  
  //TFile *f = new TFile(fin1);
  //TFile  *f = TFile::Open(fin1,"READ");
 // std::cout<< "Going to Bfinder directory " << std::endl;
  
  //TTree* t1 = (TTree*)f->Get("Bfinder/ntphi");
//  t1->Print();
  std::cout<<"Got tree ntphi"<<std::endl;
  t1->AddFriend("skimanalysis/HltTree");
  t1->AddFriend("CentWeightTree");
  std::cout<<"Got skim"<<std::endl;
  t1->AddFriend("hiEvtAnalyzer/HiTree");
  std::cout<<"Got hiEvt"<<std::endl;
  t1->AddFriend("BDT");
  t1->AddFriend("BDT_pt_5_10");  
  t1->AddFriend("BDT_pt_10_15");  
  t1->AddFriend("BDT_pt_15_20");  
  t1->AddFriend("BDT_pt_20_50");  
  std::cout<<"Added lots of friends"<<std::endl;
  readBranch(t1);
  
//  return;  
  ofstream scan_file;
  scan_file.open ("scan_file_b+.txt");
  std::cout << "Input TTree read " << std::endl;
  std::cout << "Creating new TTree " << std::endl;
  
  
  TFile *fout = new TFile(data_selection_output_file,"recreate");
  
  TTree *t4 = new TTree(channel_to_ntuple_name(channel),channel_to_ntuple_name(channel));
  Float_t pVz;
    
  Float_t mass;
  Float_t pt;
  Float_t y;
  Float_t trk1eta;
  Float_t trk2eta;
  Float_t chi2cl;
  Float_t svpvdistance, svpvdis_err;
  Float_t mumumass, trktrkmass; 
 //Float_t svpvdistance_2D, svpvdis_2D_err;
  Float_t alpha;
  //Double_t trk1D0, trk2D0, trk1D0Err, trk2D0Err;
  Float_t trk1Dz, trk1DzError;
  Float_t trk1Y;
  Float_t vtxX, vtxY;
  Float_t trk1Dxy, trk1DxyError;
  Float_t trk1pt;
  Float_t trk2pt;
  Float_t mu1pt, mu2pt;
  Float_t mu1eta, mu2eta;
  Float_t d0, d0err;
 /* Double_t lxy;*/
  Float_t  Pthatweight;
  Double_t bdt_pt_5_10;
  Double_t bdt_pt_10_15;
  Double_t bdt_pt_15_20;
  Double_t bdt_pt_20_50;
  Double_t bdt_pt_5_7;
  Double_t bdt_pt_7_10;
  Double_t bdt_pt_20_30;
  Double_t bdt_pt_30_50;
  Double_t bdt_pt_50_100;
  Float_t bgenpt;
  Double_t centweight;
  Int_t HiBin;
  std::cout << "New TTree Created " << std::endl;
  
    t4->Branch("Bmass", &mass);
    t4->Branch("Bpt", &pt);
    t4->Branch("By", &y);
    t4->Branch("Btrk1eta", &trk1eta);
    t4->Branch("Btrk1Y", &trk1Y);
    t4->Branch("Btrk2eta", &trk2eta);
    t4->Branch("Btrk1pt", &trk1pt);
    t4->Branch("Btrk2pt", &trk2pt);
    t4->Branch("Bmu1eta", &mu1eta);
    t4->Branch("Bmu2eta", &mu2eta);
    t4->Branch("Bmu1pt", &mu1pt);
    t4->Branch("Bmu2pt", &mu2pt);
    t4->Branch("Bchi2cl", &chi2cl);
    t4->Branch("Bmumumass", &mumumass);
    t4->Branch("Btrktrkmass", &trktrkmass);
 // t4->Branch("BsvpvDistance_2D", &svpvdistance_2D);
  //t4->Branch("BsvpvDistance_2D_Err", &svpvdis_2D_err);
    t4->Branch("BsvpvDistance", &svpvdistance);
    t4->Branch("BsvpvDistance_Err", &svpvdis_err);
    t4->Branch("Balpha", &alpha);
  //t4->Branch("Btrk1D0", &trk1D0);
  //t4->Branch("Btrk2D0", &trk2D0);
  //t4->Branch("Btrk1D0Err", &trk1D0Err);
  //t4->Branch("Btrk2D0Err", &trk2D0Err);
    t4->Branch("Btrk1Dz1", &trk1Dz);
    t4->Branch("BvtxX", &vtxX);
    t4->Branch("BvtxY", &vtxY);
    t4->Branch("Btrk1DzError1", &trk1DzError);
    t4->Branch("Btrk1Dxy1", &trk1Dxy);
    t4->Branch("Btrk1DxyError1", &trk1DxyError);
  //t4->Branch("Btrk2Dz", &trk2Dz);
    t4->Branch("Bd0", &d0);
    t4->Branch("Bd0err", &d0err);
  //t4->Branch("Blxy", &lxy);

    t4->Branch("Pthatweight", &Pthatweight);
    t4->Branch("HiBin", &HiBin);
    t4->Branch("PVz", &pVz);
    t4->Branch("CentWeight", &centweight);
    t4->Branch("Bgenpt", &bgenpt);
    t4->Branch("BDT_pt_5_10", &bdt_pt_5_10);
    t4->Branch("BDT_pt_10_15", &bdt_pt_10_15);
    t4->Branch("BDT_pt_15_20", &bdt_pt_15_20);
    t4->Branch("BDT_pt_20_50", &bdt_pt_20_50);
    t4->Branch("BDT_pt_5_7", &bdt_pt_5_7);
    t4->Branch("BDT_pt_7_10", &bdt_pt_7_10);
    t4->Branch("BDT_pt_20_30", &bdt_pt_20_30);
    t4->Branch("BDT_pt_30_50", &bdt_pt_30_50);
    t4->Branch("BDT_pt_50_100", &bdt_pt_50_100);
    std::cout<<"Loop over events"<<std::endl;
  
  int count=0;
  std::cout<<"before the loop"<<std::endl;
//  for (int evt=0;evt<10000;evt++) {
//   for (int evt=0;evt<10000;evt++) {
    for (int evt=0;evt<(t1->GetEntries());evt++) {
//  for (int evt=0;evt<5;evt++) {
  //  scan_file<<"*********************** EVT = "<<endl;
    t1->GetEntry(evt);
//    scan_file<<"Bsize = "<<Bsize<<endl;
    if(channel==4){  
  //ESTE    if(!((hiBin<181)&&abs(PVz)<15))continue;
    } 
        
    for(int i=0; i<Bsize; i++){

    /*scan_file<<"B nr = "<<i<<" from evt "<<evt<<endl;
    scan_file<<"Bsize = "<<Bsize<<endl;
    scan_file<<"Bmass = "<<Bmass[i]<<endl;*/
    
        if(channel==1){
         if(!((pprimaryVertexFilter&&phfCoincFilter2Th4&&pclusterCompatibilityFilter&&Btrk1Pt[i]>0.9&&Bpt[i]>5.0&&(BsvpvDistance[i]/BsvpvDisErr[i])>2.0&&Bchi2cl[i]>0.05&&TMath::Abs(Btrk1Eta[i])<2.4&&TMath::Abs(By[i])<2.4&&TMath::Abs(PVz)<15&&Bmass[i]>5&&Bmass[i]<6&&TMath::Abs(Bmumumass[i]-3.096900)<0.15&&Bmu1SoftMuID[i]&&Bmu2SoftMuID[i]&&((TMath::Abs(Bmu1eta[i])<1.2&&Bmu1pt[i]>3.5)||(TMath::Abs(Bmu1eta[i])>1.2&&TMath::Abs(Bmu1eta[i])<2.1&&Bmu1pt[i]>5.47-1.89*TMath::Abs(Bmu1eta[i]))||(TMath::Abs(Bmu1eta[i])>2.1&&TMath::Abs(Bmu1eta[i])<2.4&&Bmu1pt[i]>1.5))&&((TMath::Abs(Bmu2eta[i])<1.2&&Bmu2pt[i]>3.5)||(TMath::Abs(Bmu2eta[i])>1.2&&TMath::Abs(Bmu2eta[i])<2.1&&Bmu2pt[i]>5.47-1.89*TMath::Abs(Bmu2eta[i]))||(TMath::Abs(Bmu2eta[i])>2.1&&TMath::Abs(Bmu2eta[i])<2.4&&Bmu2pt[i]>1.5))&&Bmu1isTriggered[i]&&Bmu2isTriggered[i]&&(Btrk1PixelHit[i]+Btrk1StripHit[i])>=11&&(Btrk1Chi2ndf[i]/(Btrk1nStripLayer[i]+Btrk1nPixelLayer[i]))<0.18&&TMath::Abs(Btrk1PtErr[i]/Btrk1Pt[i])<0.1))) continue; //prefilter
// ESTE NOVO        if(!((pprimaryVertexFilter&&phfCoincFilter2Th4&&pclusterCompatibilityFilter&&Btrk1Pt[i]>0.9&&Bpt[i]>5.0&&(BsvpvDistance[i]/BsvpvDisErr[i])>2.0&&Bchi2cl[i]>0.05&&TMath::Abs(Btrk1Eta[i])<2.4&&TMath::Abs(By[i])<2.4&&TMath::Abs(PVz)<15&&Bmass[i]>5&&Bmass[i]<6&&TMath::Abs(Bmumumass[i]-3.096900)<0.15&&Bmu1SoftMuID[i]&&Bmu2SoftMuID[i]&&((TMath::Abs(Bmu1eta[i])<1.2&&Bmu1pt[i]>3.5)||(TMath::Abs(Bmu1eta[i])>1.2&&TMath::Abs(Bmu1eta[i])<2.1&&Bmu1pt[i]>5.47-1.89*TMath::Abs(Bmu1eta[i]))||(TMath::Abs(Bmu1eta[i])>2.1&&TMath::Abs(Bmu1eta[i])<2.4&&Bmu1pt[i]>1.5))&&((TMath::Abs(Bmu2eta[i])<1.2&&Bmu2pt[i]>3.5)||(TMath::Abs(Bmu2eta[i])>1.2&&TMath::Abs(Bmu2eta[i])<2.1&&Bmu2pt[i]>5.47-1.89*TMath::Abs(Bmu2eta[i]))||(TMath::Abs(Bmu2eta[i])>2.1&&TMath::Abs(Bmu2eta[i])<2.4&&Bmu2pt[i]>1.5))&&Bmu1isTriggered[i]&&Bmu2isTriggered[i]&&(Btrk1PixelHit[i]+Btrk1StripHit[i])>=11&&(Btrk1Chi2ndf[i]/(Btrk1nStripLayer[i]+Btrk1nPixelLayer[i]))<0.18&&TMath::Abs(Btrk1PtErr[i]/Btrk1Pt[i])<0.1)&&((Bpt[i]>5&&Bpt[i]<7&&(BsvpvDistance[i]/BsvpvDisErr[i])>12.0&&cos(Bdtheta[i])>0.95&&BDT_5_7[i]>0.07)||(Bpt[i]>7&&Bpt[i]<10&&BDT_7_10[i]>0.08)||(Bpt[i]>10&&Bpt[i]<15&&BDT_10_15[i]>0.09)||(Bpt[i]>15&&Bpt[i]<20&&BDT_15_20[i]>0.09)||(Bpt[i]>20&&Bpt[i]<30&&BDT_20_30[i]>0.07)||(Bpt[i]>30&&Bpt[i]<50&&BDT_30_50[i]>0.12)||(Bpt[i]>50&&Bpt[i]<100&& BDT_50_100[i]>0.24)))) continue;
//          if(!(Btrk1Pt[i]>1.0&&Bpt[i]>5.0&&(BsvpvDistance[i]/BsvpvDisErr[i])>2.0&&Bchi2cl[i]>0.05&&TMath::Abs(Btrk1Eta[i])<2.4&&TMath::Abs(By[i])<2.4&&TMath::Abs(PVz)<15&&Bmass[i]>5&&Bmass[i]<6&&Bmu1isGlobalMuon[i]&&Bmu2isGlobalMuon[i]&&Bmu1dxyPV[i]<0.3&&Bmu2dxyPV[i]<0.3&&Bmu1dzPV[i]<20&&Bmu2dzPV[i]<20&&Bmu1InPixelLayer[i]>0&&(Bmu1InPixelLayer[i]+Bmu1InStripLayer[i])>5&&Bmu2InPixelLayer[i]>0&&(Bmu2InPixelLayer[i]+Bmu2InStripLayer[i])>5&&((TMath::Abs(Bmu1eta[i])<1.2&&Bmu1pt[i]>3.5)||(TMath::Abs(Bmu1eta[i])>1.2&&TMath::Abs(Bmu1eta[i])<2.1&&Bmu1pt[i]>(5.77-1.8*TMath::Abs(Bmu1eta[i])))||(TMath::Abs(Bmu1eta[i])>2.1&&TMath::Abs(Bmu1eta[i])<2.4&&Bmu1pt[i]>1.8))&&TMath::Abs(Bmumumass[i]-3.096900)<0.15&&Bmu1TMOneStationTight[i]&&Bmu2TMOneStationTight[i]&&Btrk1highPurity[i]&&(Btrk1PixelHit[i]+Btrk1StripHit[i])>=11&&(Btrk1Chi2ndf[i]/(Btrk1nStripLayer[i]+Btrk1nPixelLayer[i]))<0.18&&TMath::Abs(Btrk1PtErr[i]/Btrk1Pt[i])<0.1&&((Bpt[i]>5&&Bpt[i]<7&&(BsvpvDistance[i]/BsvpvDisErr[i])>10.497&&cos(Bdtheta[i])>0.922&&Btrk1Pt[i]>0.983&&Bchi2cl[i]>0.064&&TMath::Abs(Btrk1Eta[i])<2.380)||(Bpt[i]>7&&Bpt[i]<10&&(BsvpvDistance[i]/BsvpvDisErr[i])>15.208&&cos(Bdtheta[i])>0.966&&Btrk1Pt[i]>0.976&&Bchi2cl[i]>0.057)||(Bpt[i]>10&&Bpt[i]<15&&(BsvpvDistance[i]/BsvpvDisErr[i])>7.091&&cos(Bdtheta[i])>0.983&&Btrk1Pt[i]>1.540&&Bchi2cl[i]>0.052)||(Bpt[i]>15&&Bpt[i]<20&&(BsvpvDistance[i]/BsvpvDisErr[i])>5.884&&cos(Bdtheta[i])>0.616&&Btrk1Pt[i]>1.336&&Bchi2cl[i]>0.055)||(Bpt[i]>20&&Bpt[i]<30&&(BsvpvDistance[i]/BsvpvDisErr[i])>3.473&&cos(Bdtheta[i])>-0.535&&Btrk1Pt[i]>1.727&&Bchi2cl[i]>0.048)||(Bpt[i]>30&&Bpt[i]<50&&cos(Bdtheta[i])>0.993&&Btrk1Pt[i]>1.577&&Bchi2cl[i]>0.050&&TMath::Abs(Btrk1Eta[i])<2.387)||(Bpt[i]>50 && Bpt[i]<100)))) continue;
//ESTE ANTIGO    if(!((pprimaryVertexFilter&&phfCoincFilter2Th4&&pclusterCompatibilityFilter&&Btrk1Pt[i]>0.9&&Bpt[i]>5.0&&(BsvpvDistance[i]/BsvpvDisErr[i])>2.0&&Bchi2cl[i]>0.05&&TMath::Abs(Btrk1Eta[i])<2.4&&TMath::Abs(By[i])<2.4&&TMath::Abs(PVz)<15&&Bmass[i]>5&&Bmass[i]<6&&TMath::Abs(Bmumumass[i]-3.096900)<0.15&&Bmu1SoftMuID[i]&&Bmu2SoftMuID[i]&&((TMath::Abs(Bmu1eta[i])<1.2&&Bmu1pt[i]>3.5)||(TMath::Abs(Bmu1eta[i])>1.2&&TMath::Abs(Bmu1eta[i])<2.1&&Bmu1pt[i]>5.47-1.89*TMath::Abs(Bmu1eta[i]))||(TMath::Abs(Bmu1eta[i])>2.1&&TMath::Abs(Bmu1eta[i])<2.4&&Bmu1pt[i]>1.5))&&((TMath::Abs(Bmu2eta[i])<1.2&&Bmu2pt[i]>3.5)||(TMath::Abs(Bmu2eta[i])>1.2&&TMath::Abs(Bmu2eta[i])<2.1&&Bmu2pt[i]>5.47-1.89*TMath::Abs(Bmu2eta[i]))||(TMath::Abs(Bmu2eta[i])>2.1&&TMath::Abs(Bmu2eta[i])<2.4&&Bmu2pt[i]>1.5))&&Bmu1isTriggered[i]&&Bmu2isTriggered[i]&&(Btrk1PixelHit[i]+Btrk1StripHit[i])>=11&&(Btrk1Chi2ndf[i]/(Btrk1nStripLayer[i]+Btrk1nPixelLayer[i]))<0.18&&TMath::Abs(Btrk1PtErr[i]/Btrk1Pt[i])<0.1)&&((Bpt[i]>5&&Bpt[i]<7&&(BsvpvDistance[i]/BsvpvDisErr[i])>16.457&&cos(Bdtheta[i])>0.987&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>0.005&&Btrk1Pt[i]>1.092&&Bchi2cl[i]>0.065)||(Bpt[i]>7&&Bpt[i]<10&&(BsvpvDistance[i]/BsvpvDisErr[i])>12.714&&cos(Bdtheta[i])>0.947&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>2.928&&Btrk1Pt[i]>0.838&&Bchi2cl[i]>0.053)||(Bpt[i]>10&&Bpt[i]<15&&(BsvpvDistance[i]/BsvpvDisErr[i])>9.086&&cos(Bdtheta[i])>0.994&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>1.540&&Btrk1Pt[i]>1.262&&Bchi2cl[i]>0.055)||(Bpt[i]>15&&Bpt[i]<20&&(BsvpvDistance[i]/BsvpvDisErr[i])>7.587&&cos(Bdtheta[i])>0.757&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>0.00&&Btrk1Pt[i]>1.813&&Bchi2cl[i]>0.056)||(Bpt[i]>20&&Bpt[i]<30&&(BsvpvDistance[i]/BsvpvDisErr[i])>4.004&&cos(Bdtheta[i])>0.996&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>0.000&&Btrk1Pt[i]>1.822&&Bchi2cl[i]>0.050)||(Bpt[i]>30&&Bpt[i]<50&&(BsvpvDistance[i]/BsvpvDisErr[i])>2.000&&cos(Bdtheta[i])>0.998&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>0.000&&Btrk1Pt[i]>2.046&&Bchi2cl[i]>0.050)||(Bpt[i]>50&&Bpt[i]<100&&(BsvpvDistance[i]/BsvpvDisErr[i])>4.084&&cos(Bdtheta[i])>-0.112&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>0.000&&Btrk1Pt[i]>1.645&&Bchi2cl[i]>0.050)))) continue;
//          if(!((pprimaryVertexFilter&&phfCoincFilter2Th4&&pclusterCompatibilityFilter&&Btrk1Pt[i]>0.9&&Bpt[i]>5.0&&(BsvpvDistance[i]/BsvpvDisErr[i])>2.0&&Bchi2cl[i]>0.05&&TMath::Abs(Btrk1Eta[i])<2.4&&TMath::Abs(By[i])<2.4&&TMath::Abs(PVz)<15&&Bmass[i]>5&&Bmass[i]<6&&TMath::Abs(Bmumumass[i]-3.096900)<0.15&&Bmu1SoftMuID[i]&&Bmu2SoftMuID[i]&&((TMath::Abs(Bmu1eta[i])<1.2&&Bmu1pt[i]>3.5)||(TMath::Abs(Bmu1eta[i])>1.2&&TMath::Abs(Bmu1eta[i])<2.1&&Bmu1pt[i]>5.47-1.89*TMath::Abs(Bmu1eta[i]))||(TMath::Abs(Bmu1eta[i])>2.1&&TMath::Abs(Bmu1eta[i])<2.4&&Bmu1pt[i]>1.5))&&((TMath::Abs(Bmu2eta[i])<1.2&&Bmu2pt[i]>3.5)||(TMath::Abs(Bmu2eta[i])>1.2&&TMath::Abs(Bmu2eta[i])<2.1&&Bmu2pt[i]>5.47-1.89*TMath::Abs(Bmu2eta[i]))||(TMath::Abs(Bmu2eta[i])>2.1&&TMath::Abs(Bmu2eta[i])<2.4&&Bmu2pt[i]>1.5))&&Bmu1isTriggered[i]&&Bmu2isTriggered[i]&&(Btrk1PixelHit[i]+Btrk1StripHit[i])>=11&&(Btrk1Chi2ndf[i]/(Btrk1nStripLayer[i]+Btrk1nPixelLayer[i]))<0.18&&TMath::Abs(Btrk1PtErr[i]/Btrk1Pt[i])<0.1))&&((Bpt[i]>5&&Bpt[i]<7&&(BsvpvDistance[i]/BsvpvDisErr[i])>16.457&&cos(Bdtheta[i])>0.987&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>0.005&&Btrk1Pt[i]>1.092&&Bchi2cl[i]>0.065)||(Bpt[i]>7&&Bpt[i]<10&&(BsvpvDistance[i]/BsvpvDisErr[i])>12.714&&cos(Bdtheta[i])>0.947&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>2.928&&Btrk1Pt[i]>0.838&&Bchi2cl[i]>0.053)||(Bpt[i]>10&&Bpt[i]<15&&(BsvpvDistance[i]/BsvpvDisErr[i])>9.086&&cos(Bdtheta[i])>0.994&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>1.540&&Btrk1Pt[i]>1.262&&Bchi2cl[i]>0.055)||(Bpt[i]>15&&Bpt[i]<20&&(BsvpvDistance[i]/BsvpvDisErr[i])>7.587&&cos(Bdtheta[i])>0.757&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>0.000&&Btrk1Pt[i]>1.813&&Bchi2cl[i]>0.056)||(Bpt[i]>20&&Bpt[i]<30&&(BsvpvDistance[i]/BsvpvDisErr[i])>4.004&&cos(Bdtheta[i])>0.996&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>0.000&&Btrk1Pt[i]>1.822&&Bchi2cl[i]>0.050)||(Bpt[i]>30&&Bpt[i]<50&&(BsvpvDistance[i]/BsvpvDisErr[i])>2.000&&cos(Bdtheta[i])>0.998&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>0.000&&Btrk1Pt[i]>2.046&&Bchi2cl[i]>0.050)||(Bpt[i]>50&&Bpt[i]<100&&(BsvpvDistance[i]/BsvpvDisErr[i])>4.084&&cos(Bdtheta[i])>-0.112&&TMath::Abs(Btrk1Dxy1[i]/Btrk1DxyError1[i])>0.000&&Btrk1Pt[i]>1.645&&Bchi2cl[i]>0.050))) continue;
          if(mc==1&&Bgen[i]!=23333) continue;
        }        
        
        if(channel==4){
          if(mc==0){  
             if(!((hiBin<181)&&Btrk1Pt[i]>1.0&&Btrk2Pt[i]>1.0&&Bchi2cl[i]>0.05&&BsvpvDistance[i]/BsvpvDisErr[i]>2.2&&Bpt[i]>5&&abs(Btrk1Eta[i]-0.0)<2.4&&abs(Btrk2Eta[i]-0.0)<2.4&&(TMath::Abs(By[i])<2.4&&TMath::Abs(Bmumumass[i]-3.096916)<0.15&&((abs(Bmu1eta[i])<1.2&&Bmu1pt[i]>3.5)||(abs(Bmu1eta[i])>1.2&&abs(Bmu1eta[i])<2.1&&Bmu1pt[i]>(5.77-1.8*abs(Bmu1eta[i])))||(abs(Bmu1eta[i])>2.1&&abs(Bmu1eta[i])<2.4&&Bmu1pt[i]>1.8))&&((abs(Bmu2eta[i])<1.2&&Bmu2pt[i]>3.5)||(abs(Bmu2eta[i])>1.2&&abs(Bmu2eta[i])<2.1&&Bmu2pt[i]>(5.77-1.8*abs(Bmu2eta[i])))||(abs(Bmu2eta[i])>2.1&&abs(Bmu2eta[i])<2.4&&Bmu2pt[i]>1.8))&&Bmu1TMOneStationTight[i]&&Bmu2TMOneStationTight[i]&&Bmu1InPixelLayer[i]>0&&(Bmu1InPixelLayer[i]+Bmu1InStripLayer[i])>5&&Bmu2InPixelLayer[i]>0&&(Bmu2InPixelLayer[i]+Bmu2InStripLayer[i])>5&&Bmu1dxyPV[i]<0.3&&Bmu2dxyPV[i]<0.3&&Bmu1dzPV[i]<20&&Bmu2dzPV[i]<20&&Bmu1isGlobalMuon[i]&&Bmu2isGlobalMuon[i]&&Btrk1highPurity[i]&&Btrk2highPurity[i]&&abs(Btrk1Eta[i])<2.4&&abs(Btrk2Eta[i])<2.4&&Btrk1Pt[i]>1.&&Btrk2Pt[i]>1.&&abs(Btktkmass[i]-1.019455)<0.015)&&(abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter)&&(Btrk1PixelHit[i]+Btrk1StripHit[i]>10)&&(Btrk2PixelHit[i]+Btrk2StripHit[i]>10)&&(Btrk1PtErr[i]/Btrk1Pt[i]<0.1)&&(Btrk2PtErr[i]/Btrk2Pt[i]<0.1)&&Btrk1Chi2ndf[i]/(Btrk1nStripLayer[i]+Btrk1nPixelLayer[i])<0.18&&Btrk2Chi2ndf[i]/(Btrk2nStripLayer[i]+Btrk2nPixelLayer[i])<0.18&&((Bpt[i]>5&&Bpt[i]<10&&BDT_pt_5_10[i]>0.32)||(Bpt[i]>10&&Bpt[i]<15&&BDT_pt_10_15[i]>0.29)||(Bpt[i]>15&&Bpt[i]<20&&BDT_pt_15_20[i]>0.35)||(Bpt[i]>20&&Bpt[i]<50&&BDT_pt_20_50[i]>0.33)))) continue; //NEW TRAINING

//              if(!((hiBin<181)&&Btrk1Pt[i]>1.0&&Btrk2Pt[i]>1.0&&Bchi2cl[i]>0.05&&BsvpvDistance[i]/BsvpvDisErr[i]>2.2&&Bpt[i]>5&&abs(Btrk1Eta[i]-0.0)<2.4&&abs(Btrk2Eta[i]-0.0)<2.4&&(TMath::Abs(By[i])<2.4&&TMath::Abs(Bmumumass[i]-3.096916)<0.15&&((abs(Bmu1eta[i])<1.2&&Bmu1pt[i]>3.5)||(abs(Bmu1eta[i])>1.2&&abs(Bmu1eta[i])<2.1&&Bmu1pt[i]>(5.77-1.8*abs(Bmu1eta[i])))||(abs(Bmu1eta[i])>2.1&&abs(Bmu1eta[i])<2.4&&Bmu1pt[i]>1.8))&&((abs(Bmu2eta[i])<1.2&&Bmu2pt[i]>3.5)||(abs(Bmu2eta[i])>1.2&&abs(Bmu2eta[i])<2.1&&Bmu2pt[i]>(5.77-1.8*abs(Bmu2eta[i])))||(abs(Bmu2eta[i])>2.1&&abs(Bmu2eta[i])<2.4&&Bmu2pt[i]>1.8))&&Bmu1TMOneStationTight[i]&&Bmu2TMOneStationTight[i]&&Bmu1InPixelLayer[i]>0&&(Bmu1InPixelLayer[i]+Bmu1InStripLayer[i])>5&&Bmu2InPixelLayer[i]>0&&(Bmu2InPixelLayer[i]+Bmu2InStripLayer[i])>5&&Bmu1dxyPV[i]<0.3&&Bmu2dxyPV[i]<0.3&&Bmu1dzPV[i]<20&&Bmu2dzPV[i]<20&&Bmu1isGlobalMuon[i]&&Bmu2isGlobalMuon[i]&&Btrk1highPurity[i]&&Btrk2highPurity[i]&&abs(Btrk1Eta[i])<2.4&&abs(Btrk2Eta[i])<2.4&&Btrk1Pt[i]>1.&&Btrk2Pt[i]>1.&&abs(Btktkmass[i]-1.019455)<0.015)&&(abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter)&&(Btrk1PixelHit[i]+Btrk1StripHit[i]>10)&&(Btrk2PixelHit[i]+Btrk2StripHit[i]>10)&&(Btrk1PtErr[i]/Btrk1Pt[i]<0.1)&&(Btrk2PtErr[i]/Btrk2Pt[i]<0.1)&&Btrk1Chi2ndf[i]/(Btrk1nStripLayer[i]+Btrk1nPixelLayer[i])<0.18&&Btrk2Chi2ndf[i]/(Btrk2nStripLayer[i]+Btrk2nPixelLayer[i])<0.18&&((Bpt[i]>5&&Bpt[i]<10&&BDT_pt_5_10[i]>0.17)||(Bpt[i]>10&&Bpt[i]<15&&BDT_pt_10_15[i]>0.14)||(Bpt[i]>15&&Bpt[i]<20&&BDT_pt_15_20[i]>0.2)||(Bpt[i]>20&&Bpt[i]<50&&BDT_pt_20_50[i]>0.2)))) continue;
//Old training              if(!((hiBin<181)&&Btrk1Pt[i]>1.0&&Btrk2Pt[i]>1.0&&Bchi2cl[i]>0.05&&BsvpvDistance[i]/BsvpvDisErr[i]>2.2&&Bpt[i]>5&&abs(Btrk1Eta[i]-0.0)<2.4&&abs(Btrk2Eta[i]-0.0)<2.4&&(TMath::Abs(By[i])<2.4&&TMath::Abs(Bmumumass[i]-3.096916)<0.15&&((abs(Bmu1eta[i])<1.2&&Bmu1pt[i]>3.5)||(abs(Bmu1eta[i])>1.2&&abs(Bmu1eta[i])<2.1&&Bmu1pt[i]>(5.77-1.8*abs(Bmu1eta[i])))||(abs(Bmu1eta[i])>2.1&&abs(Bmu1eta[i])<2.4&&Bmu1pt[i]>1.8))&&((abs(Bmu2eta[i])<1.2&&Bmu2pt[i]>3.5)||(abs(Bmu2eta[i])>1.2&&abs(Bmu2eta[i])<2.1&&Bmu2pt[i]>(5.77-1.8*abs(Bmu2eta[i])))||(abs(Bmu2eta[i])>2.1&&abs(Bmu2eta[i])<2.4&&Bmu2pt[i]>1.8))&&Bmu1TMOneStationTight[i]&&Bmu2TMOneStationTight[i]&&Bmu1InPixelLayer[i]>0&&(Bmu1InPixelLayer[i]+Bmu1InStripLayer[i])>5&&Bmu2InPixelLayer[i]>0&&(Bmu2InPixelLayer[i]+Bmu2InStripLayer[i])>5&&Bmu1dxyPV[i]<0.3&&Bmu2dxyPV[i]<0.3&&Bmu1dzPV[i]<20&&Bmu2dzPV[i]<20&&Bmu1isGlobalMuon[i]&&Bmu2isGlobalMuon[i]&&Btrk1highPurity[i]&&Btrk2highPurity[i]&&abs(Btrk1Eta[i])<2.4&&abs(Btrk2Eta[i])<2.4&&Btrk1Pt[i]>1.&&Btrk2Pt[i]>1.&&abs(Btktkmass[i]-1.019455)<0.015)&&(abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter)&&(Btrk1PixelHit[i]+Btrk1StripHit[i]>10)&&(Btrk2PixelHit[i]+Btrk2StripHit[i]>10)&&(Btrk1PtErr[i]/Btrk1Pt[i]<0.1)&&(Btrk2PtErr[i]/Btrk2Pt[i]<0.1)&&Btrk1Chi2ndf[i]/(Btrk1nStripLayer[i]+Btrk1nPixelLayer[i])<0.18&&Btrk2Chi2ndf[i]/(Btrk2nStripLayer[i]+Btrk2nPixelLayer[i])<0.18&&((Bpt[i]>5&&Bpt[i]<10&&BDT_pt_5_10[i]>0.17)||(Bpt[i]>10&&Bpt[i]<15&&BDT_pt_10_15[i]>0.17)||(Bpt[i]>15&&Bpt[i]<20&&BDT_pt_15_20[i]>0.26)||(Bpt[i]>20&&Bpt[i]<50&&BDT_pt_20_50[i]>0.25)))) continue;
          }
          if(mc==1){
//              if(!((hiBin<181)&&Btrk1Pt[i]>1.0&&Btrk2Pt[i]>1.0&&Bchi2cl[i]>0.05&&BsvpvDistance[i]/BsvpvDisErr[i]>2.2&&Bpt[i]>5&&abs(Btrk1Eta[i]-0.0)<2.4&&abs(Btrk2Eta[i]-0.0)<2.4&&(TMath::Abs(By[i])<2.4&&TMath::Abs(Bmumumass[i]-3.096916)<0.15&&((abs(Bmu1eta[i])<1.2&&Bmu1pt[i]>3.5)||(abs(Bmu1eta[i])>1.2&&abs(Bmu1eta[i])<2.1&&Bmu1pt[i]>(5.77-1.8*abs(Bmu1eta[i])))||(abs(Bmu1eta[i])>2.1&&abs(Bmu1eta[i])<2.4&&Bmu1pt[i]>1.8))&&((abs(Bmu2eta[i])<1.2&&Bmu2pt[i]>3.5)||(abs(Bmu2eta[i])>1.2&&abs(Bmu2eta[i])<2.1&&Bmu2pt[i]>(5.77-1.8*abs(Bmu2eta[i])))||(abs(Bmu2eta[i])>2.1&&abs(Bmu2eta[i])<2.4&&Bmu2pt[i]>1.8))&&Bmu1TMOneStationTight[i]&&Bmu2TMOneStationTight[i]&&Bmu1InPixelLayer[i]>0&&(Bmu1InPixelLayer[i]+Bmu1InStripLayer[i])>5&&Bmu2InPixelLayer[i]>0&&(Bmu2InPixelLayer[i]+Bmu2InStripLayer[i])>5&&Bmu1dxyPV[i]<0.3&&Bmu2dxyPV[i]<0.3&&Bmu1dzPV[i]<20&&Bmu2dzPV[i]<20&&Bmu1isGlobalMuon[i]&&Bmu2isGlobalMuon[i]&&Btrk1highPurity[i]&&Btrk2highPurity[i]&&abs(Btrk1Eta[i])<2.4&&abs(Btrk2Eta[i])<2.4&&Btrk1Pt[i]>1.&&Btrk2Pt[i]>1.&&abs(Btktkmass[i]-1.019455)<0.015)&&(abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter)&&(Btrk1PixelHit[i]+Btrk1StripHit[i]>10)&&(Btrk2PixelHit[i]+Btrk2StripHit[i]>10)&&(Btrk1PtErr[i]/Btrk1Pt[i]<0.1)&&(Btrk2PtErr[i]/Btrk2Pt[i]<0.1)&&Btrk1Chi2ndf[i]/(Btrk1nStripLayer[i]+Btrk1nPixelLayer[i])<0.18&&Btrk2Chi2ndf[i]/(Btrk2nStripLayer[i]+Btrk2nPixelLayer[i])<0.18&&((Bpt[i]>5&&Bpt[i]<10&&BDT_pt_5_10[i]>0.17)||(Bpt[i]>10&&Bpt[i]<15&&BDT_pt_10_15[i]>0.14)||(Bpt[i]>15&&Bpt[i]<20&&BDT_pt_15_20[i]>0.2)||(Bpt[i]>20&&Bpt[i]<50&&BDT_pt_20_50[i]>0.2))&&Bgen[i]==23333)) continue;
              if(!((hiBin<181)&&Btrk1Pt[i]>1.0&&Btrk2Pt[i]>1.0&&Bchi2cl[i]>0.05&&BsvpvDistance[i]/BsvpvDisErr[i]>2.2&&Bpt[i]>5&&abs(Btrk1Eta[i]-0.0)<2.4&&abs(Btrk2Eta[i]-0.0)<2.4&&(TMath::Abs(By[i])<2.4&&TMath::Abs(Bmumumass[i]-3.096916)<0.15&&((abs(Bmu1eta[i])<1.2&&Bmu1pt[i]>3.5)||(abs(Bmu1eta[i])>1.2&&abs(Bmu1eta[i])<2.1&&Bmu1pt[i]>(5.77-1.8*abs(Bmu1eta[i])))||(abs(Bmu1eta[i])>2.1&&abs(Bmu1eta[i])<2.4&&Bmu1pt[i]>1.8))&&((abs(Bmu2eta[i])<1.2&&Bmu2pt[i]>3.5)||(abs(Bmu2eta[i])>1.2&&abs(Bmu2eta[i])<2.1&&Bmu2pt[i]>(5.77-1.8*abs(Bmu2eta[i])))||(abs(Bmu2eta[i])>2.1&&abs(Bmu2eta[i])<2.4&&Bmu2pt[i]>1.8))&&Bmu1TMOneStationTight[i]&&Bmu2TMOneStationTight[i]&&Bmu1InPixelLayer[i]>0&&(Bmu1InPixelLayer[i]+Bmu1InStripLayer[i])>5&&Bmu2InPixelLayer[i]>0&&(Bmu2InPixelLayer[i]+Bmu2InStripLayer[i])>5&&Bmu1dxyPV[i]<0.3&&Bmu2dxyPV[i]<0.3&&Bmu1dzPV[i]<20&&Bmu2dzPV[i]<20&&Bmu1isGlobalMuon[i]&&Bmu2isGlobalMuon[i]&&Btrk1highPurity[i]&&Btrk2highPurity[i]&&abs(Btrk1Eta[i])<2.4&&abs(Btrk2Eta[i])<2.4&&Btrk1Pt[i]>1.&&Btrk2Pt[i]>1.&&abs(Btktkmass[i]-1.019455)<0.015)&&(abs(PVz)<15&&pclusterCompatibilityFilter&&pprimaryVertexFilter)&&(Btrk1PixelHit[i]+Btrk1StripHit[i]>10)&&(Btrk2PixelHit[i]+Btrk2StripHit[i]>10)&&(Btrk1PtErr[i]/Btrk1Pt[i]<0.1)&&(Btrk2PtErr[i]/Btrk2Pt[i]<0.1)&&Btrk1Chi2ndf[i]/(Btrk1nStripLayer[i]+Btrk1nPixelLayer[i])<0.18&&Btrk2Chi2ndf[i]/(Btrk2nStripLayer[i]+Btrk2nPixelLayer[i])<0.18&&((Bpt[i]>5&&Bpt[i]<10&&BDT_pt_5_10[i]>0.32)||(Bpt[i]>10&&Bpt[i]<15&&BDT_pt_10_15[i]>0.29)||(Bpt[i]>15&&Bpt[i]<20&&BDT_pt_15_20[i]>0.35)||(Bpt[i]>20&&Bpt[i]<50&&BDT_pt_20_50[i]>0.33))&&Bgen[i]==23333)) continue; //NEW_TRAINING
          }

        }
       // scan_file<<"IT PASSED "<<endl;
        
      // if(i==0) pvz=PVz;
      
      y=By[i];
 
      trk1eta=Btrk1Eta[i];
      chi2cl=Bchi2cl[i];
      svpvdis_err=BsvpvDisErr[i];
      svpvdistance=BsvpvDistance[i];
      mumumass=Bmumumass[i];
      trktrkmass=Btktkmass[i];
    //  svpvdistance_2D=BsvpvDistance_2D[i];
     // svpvdis_2D_err=BsvpvDisErr_2D[i];
      alpha=Balpha[i];
     if(channel==1){ 
        trk1Dz=Btrk1Dz1[i];
        trk1DzError=Btrk1DzError1[i];
        trk1Dxy=Btrk1Dxy1[i];
        trk1DxyError=Btrk1DxyError1[i];
        d0= Bd0[i];
        d0err= Bd0Err[i];
        trk1Y = Btrk1Y[i];
        vtxX = BvtxX[i];
        vtxY = BvtxY[i];
        trk2eta=0;
        trk2pt=0;
        bdt_pt_5_7=BDT_5_7[i];
        bdt_pt_7_10=BDT_7_10[i];
        bdt_pt_10_15=BDT_10_15[i];
        bdt_pt_15_20=BDT_15_20[i];
        bdt_pt_20_30=BDT_20_30[i];
        bdt_pt_30_50=BDT_30_50[i];
        bdt_pt_50_100=BDT_50_100[i];
        bdt_pt_5_10=0;
        bdt_pt_20_50=0;

      }
     if(channel==4){ 
        trk1Dz=0;
        trk1DzError=0;
        trk1Dxy=0;
        trk1DxyError=0;
        d0= 0;
        d0err= 0;
        trk1Y = 0;
        vtxX = 0;
        trk2eta=Btrk2Eta[i];
        trk2pt=Btrk2Pt[i];
        vtxY = 0;
        bdt_pt_5_10=BDT_pt_5_10[i];
        bdt_pt_10_15=BDT_pt_10_15[i];
        bdt_pt_15_20=BDT_pt_15_20[i];
        bdt_pt_20_50=BDT_pt_20_50[i];
        bdt_pt_5_7=0;
        bdt_pt_7_10=0;
        bdt_pt_20_30=0;
        bdt_pt_30_50=0;
        bdt_pt_50_100=0;
      }
      /*trk1Dz=Btrk1Dz[i];
      trk2Dz=Btrk2Dz[i];*/
      trk1pt=Btrk1Pt[i];
      mu1pt=Bmu1pt[i];
      mu2pt=Bmu2pt[i];
      mu1eta=Bmu1eta[i];
      mu2eta=Bmu2eta[i];
      pVz=PVz;
//      lxy= Blxy[i];
      count++;
     // scan_file<<"WILL FILL"<<endl;
      mass=Bmass[i];
      scan_file<<"* "<<evt<< " * "<<i<<" * "<<setprecision(8)<<mass<<" * "<<endl;
//      cout<<evt<< " * "<<i<<" * "<<setprecision(7)<<mass<<endl;
     // scan_file<<"Bsize again just in case = "<<Bsize<<endl;
      pt=Bpt[i];
      HiBin=hiBin;
      if(mc==1) {
        Pthatweight=pthatweight;
        centweight=CentWeight;
        bgenpt=Bgenpt[i];
      }
      if(mc==0){
         Pthatweight=0;
         centweight=0;
         bgenpt=0;
      }
      t4->Fill();
    //  scan_file<<"DONE FILLIN"<<endl;
    }
  }
  std::cout<<"Selection Over"<<std::endl;
  std::cout<<"count= "<<count<<std::endl;
  
  
  t4->Write();
  f->Close();
  fout->Close();
  scan_file.close();
}
