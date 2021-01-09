#include "../PlotHelper/tdrstyle.C"
#include "../PlotHelper/CMS_lumi.C"

void ChiChiPlotMaker(int Mass = 20) {
  setTDRStyle();

  writeExtraText = true;
  extraText = "DELPHES SIM";
  lumi_sqrtS = "35.9 fb^{-1} (13 TeV)";

  int iPeriod = 0;
  int iPos = 11;

  TString str_Mass = TString::Itoa(Mass, 10);

  TFile *File = new TFile("../ROOT_M"+str_Mass+".root");
  TH1D *Hist = (TH1D*)File->Get("ChiChi");

  TCanvas *Canvas = new TCanvas("Canvas","",1000,1000);
  Canvas->cd();

  Hist->SetStats(0);
  Hist->Draw("colz");

  //Hist->SetTitle("Recasting [CMS-HIG-18-011]");
  Hist->SetTitle("");

  TLatex *tex = new TLatex(.12,.15,"m_{a_{1}} = "+str_Mass+" GeV");
  tex->SetNDC();
  tex->SetLineWidth(2);
  tex->Draw();

  int nColors = 10;
  Int_t Colors[10] = {19,18,17,16,15,14,13,12,kGray+3,kBlack};

  Canvas->SetRightMargin(0.1);
  //Canvas->GetCanvas()->SetGrayscale();
  //gStyle->SetPalette(52);
  gStyle->SetPalette(nColors,Colors);

  CMS_lumi( Canvas , iPeriod , iPos );

  Canvas->SaveAs("../plots/ChiChi_M"+str_Mass+".png");
  Canvas->SaveAs("../plots/ChiChi_M"+str_Mass+".pdf");
}
