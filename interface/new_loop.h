#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TMath.h>
#include <TString.h>
#include <TNtuple.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <iostream>
#include <iomanip>
#include <cmath>


#define MAX_XB 12000

Int_t    Bsize;
Float_t pthatweight;
Double_t CentWeight;
Int_t hiBin;
Float_t   PVz;
Float_t   Bmass[MAX_XB];
Float_t   Bpt[MAX_XB];
Float_t   By[MAX_XB];
Float_t   Balpha[MAX_XB];
Float_t   Bchi2cl[MAX_XB];

Float_t   Bmu1pt[MAX_XB];
Float_t   Bmu2pt[MAX_XB];
Float_t   Bmu1eta[MAX_XB];
Float_t   Bmu2eta[MAX_XB];
Float_t   Bmu1dzPV[MAX_XB];
Float_t   Bmu2dzPV[MAX_XB];
Float_t   Bmu1dxyPV[MAX_XB];
Float_t   Bmu2dxyPV[MAX_XB];
Bool_t     Bmu1isGlobalMuon[MAX_XB];
Bool_t     Bmu2isGlobalMuon[MAX_XB];
Bool_t     Bmu1TMOneStationTight[MAX_XB];
Bool_t     Bmu2TMOneStationTight[MAX_XB];
Int_t    pclusterCompatibilityFilter;
Int_t    phfCoincFilter2Th4;
Int_t    pprimaryVertexFilter;
Int_t    Bmu1InPixelLayer[MAX_XB];
Int_t    Bmu2InPixelLayer[MAX_XB];
Int_t    Bmu1InStripLayer[MAX_XB];
Int_t    Bmu2InStripLayer[MAX_XB];

Bool_t    Bmu1SoftMuID[MAX_XB];
Bool_t    Bmu2SoftMuID[MAX_XB];
Bool_t    Bmu1isTriggered[MAX_XB];
Bool_t    Bmu2isTriggered[MAX_XB];


Float_t   Bmumumass[MAX_XB];
Float_t   Btrk1Pt[MAX_XB];
Float_t   Btrk2Pt[MAX_XB];
Float_t   Btrk1Eta[MAX_XB];
Float_t   Btrk2Eta[MAX_XB];
Float_t   Btrk1PtErr[MAX_XB];
Float_t   Btrk2PtErr[MAX_XB];


Float_t   Btrk1PixelHit[MAX_XB];
Float_t   Btrk2PixelHit[MAX_XB];
Float_t   Btrk1StripHit[MAX_XB];
Float_t   Btrk2StripHit[MAX_XB];
Float_t   Btrk1nPixelLayer[MAX_XB];
Float_t   Btrk2nPixelLayer[MAX_XB];
Float_t   Btrk1nStripLayer[MAX_XB];
Float_t   Btrk2nStripLayer[MAX_XB];
Float_t   Btrk1Chi2ndf[MAX_XB];
Float_t   Btrk2Chi2ndf[MAX_XB];
Bool_t     Btrk1highPurity[MAX_XB];
Bool_t     Btrk2highPurity[MAX_XB];

Float_t   Btktkmass[MAX_XB];

Float_t   Bgen[MAX_XB];
Float_t   Bgenpt[MAX_XB];

Float_t   BsvpvDistance[MAX_XB];
Float_t   BsvpvDisErr[MAX_XB];

Double_t   BDT_pt_5_10[MAX_XB];
Double_t  BDT_pt_10_15[MAX_XB];
Double_t  BDT_pt_15_20[MAX_XB];
Double_t  BDT_pt_20_50[MAX_XB];
Double_t   BDT_5_7[MAX_XB];
Double_t  BDT_7_10[MAX_XB];
Double_t  BDT_10_15[MAX_XB];
Double_t  BDT_15_20[MAX_XB];
Double_t  BDT_20_30[MAX_XB];
Double_t  BDT_30_50[MAX_XB];
Double_t  BDT_50_100[MAX_XB];

//Adicionados para o B+

Float_t   Bdtheta[MAX_XB];
Float_t   Btrk1Y[MAX_XB];
Float_t   Btrk1Dxy1[MAX_XB];
Float_t   Btrk1DxyError1[MAX_XB];
Float_t   Btrk1Dz1[MAX_XB];
Float_t   Btrk1DzError1[MAX_XB];

Float_t   BvtxX[MAX_XB];
Float_t   BvtxY[MAX_XB];

Float_t   Bd0Err[MAX_XB];
Float_t   Bd0[MAX_XB];

//71
void readBranch(TTree* nt)
{
  nt->SetBranchAddress("Bsize",&Bsize);
  nt->SetBranchAddress("pthatweight",&pthatweight);
  nt->SetBranchAddress("CentWeight",&CentWeight);
  nt->SetBranchAddress("hiBin",&hiBin);
  nt->SetBranchAddress("PVz",&PVz);

  nt->SetBranchAddress("Bmass",Bmass);
  nt->SetBranchAddress("Bpt",Bpt);
  nt->SetBranchAddress("By",By);
  nt->SetBranchAddress("Balpha",Balpha);
  nt->SetBranchAddress("Bchi2cl",Bchi2cl);

  nt->SetBranchAddress("Btrk1Pt",Btrk1Pt);
  nt->SetBranchAddress("Btrk2Pt",Btrk2Pt);
  nt->SetBranchAddress("Btrk1Eta",Btrk1Eta);  
  nt->SetBranchAddress("Btrk2Eta",Btrk2Eta);  
  nt->SetBranchAddress("Btrk1PtErr",Btrk1PtErr); 
  nt->SetBranchAddress("Btrk2PtErr",Btrk2PtErr);
  nt->SetBranchAddress("Btrk1PixelHit",Btrk1PixelHit);
  nt->SetBranchAddress("Btrk2PixelHit",Btrk2PixelHit);
  nt->SetBranchAddress("Btrk1StripHit",Btrk1StripHit);
  nt->SetBranchAddress("Btrk2StripHit",Btrk2StripHit);
  nt->SetBranchAddress("Btrk1nPixelLayer",Btrk1nPixelLayer);
  nt->SetBranchAddress("Btrk2nPixelLayer",Btrk2nPixelLayer);
  nt->SetBranchAddress("Btrk1nStripLayer",Btrk1nStripLayer);
  nt->SetBranchAddress("Btrk2nStripLayer",Btrk2nStripLayer);
  nt->SetBranchAddress("Btrk1Chi2ndf",Btrk1Chi2ndf);
  nt->SetBranchAddress("Btrk2Chi2ndf",Btrk2Chi2ndf);
  nt->SetBranchAddress("Btrk1highPurity",Btrk1highPurity);
  nt->SetBranchAddress("Btrk2highPurity",Btrk2highPurity);


  nt->SetBranchAddress("pclusterCompatibilityFilter", &pclusterCompatibilityFilter); 

  nt->SetBranchAddress("pprimaryVertexFilter", &pprimaryVertexFilter); 
  nt->SetBranchAddress("phfCoincFilter2Th4", &phfCoincFilter2Th4); 



  nt->SetBranchAddress("Btktkmass",Btktkmass);
  nt->SetBranchAddress("Bmu1pt",Bmu1pt);
  nt->SetBranchAddress("Bmu2pt",Bmu2pt);
  nt->SetBranchAddress("Bmu1eta",Bmu1eta);
  nt->SetBranchAddress("Bmu2eta",Bmu2eta);
  nt->SetBranchAddress("Bmu1dzPV",Bmu1dzPV);
  nt->SetBranchAddress("Bmu2dzPV",Bmu2dzPV);
  nt->SetBranchAddress("Bmu1dxyPV",Bmu1dxyPV);
  nt->SetBranchAddress("Bmu2dxyPV",Bmu2dxyPV);
  nt->SetBranchAddress("Bmu1isGlobalMuon",Bmu1isGlobalMuon);
  nt->SetBranchAddress("Bmu2isGlobalMuon",Bmu2isGlobalMuon);
  nt->SetBranchAddress("Bmu1TMOneStationTight",Bmu1TMOneStationTight);
  nt->SetBranchAddress("Bmu2TMOneStationTight",Bmu2TMOneStationTight);
  nt->SetBranchAddress("Bmu1InPixelLayer",Bmu1InPixelLayer);
  nt->SetBranchAddress("Bmu2InPixelLayer",Bmu2InPixelLayer);
  nt->SetBranchAddress("Bmu1InStripLayer",Bmu1InStripLayer);
  nt->SetBranchAddress("Bmu2InStripLayer",Bmu2InStripLayer);

  nt->SetBranchAddress("Bmu1SoftMuID",Bmu1SoftMuID);
  nt->SetBranchAddress("Bmu2SoftMuID",Bmu2SoftMuID);
  nt->SetBranchAddress("Bmu1isTriggered",Bmu1isTriggered);
  nt->SetBranchAddress("Bmu2isTriggered",Bmu2isTriggered);


 nt->SetBranchAddress("Bmumumass",Bmumumass);


nt->SetBranchAddress("Bgen",Bgen);
nt->SetBranchAddress("Bgenpt",Bgenpt);

nt->SetBranchAddress("BsvpvDistance",BsvpvDistance);
nt->SetBranchAddress("BsvpvDisErr",BsvpvDisErr);
nt->SetBranchAddress("Bdtheta",Bdtheta);
nt->SetBranchAddress("Bd0",Bd0);
nt->SetBranchAddress("Bd0Err",Bd0Err);
nt->SetBranchAddress("BvtxX",BvtxX);
nt->SetBranchAddress("BvtxY",BvtxY);
nt->SetBranchAddress("Btrk1Y",Btrk1Y);
nt->SetBranchAddress("Btrk1Dxy1",Btrk1Dxy1);
nt->SetBranchAddress("Btrk1DxyError1",Btrk1DxyError1);
nt->SetBranchAddress("Btrk1Dz1",Btrk1Dz1);
nt->SetBranchAddress("Btrk1DzError1",Btrk1DzError1);
nt->SetBranchAddress("BDT_pt_5_10", BDT_pt_5_10);
nt->SetBranchAddress("BDT_pt_10_15", BDT_pt_10_15);
nt->SetBranchAddress("BDT_pt_15_20", BDT_pt_15_20);
nt->SetBranchAddress("BDT_pt_20_50", BDT_pt_20_50);
nt->SetBranchAddress("BDT_5_7", BDT_5_7);
nt->SetBranchAddress("BDT_7_10", BDT_7_10);
nt->SetBranchAddress("BDT_10_15", BDT_10_15);
nt->SetBranchAddress("BDT_15_20", BDT_15_20);
nt->SetBranchAddress("BDT_20_30", BDT_20_30);
nt->SetBranchAddress("BDT_30_50", BDT_30_50);
nt->SetBranchAddress("BDT_50_100", BDT_50_100);

}

