#include "../../PlotHelper/tdrstyle.C"
#include "../../PlotHelper/CMS_lumi.C"

void bbRes(double Mass_a1 = 40.) {
  setTDRStyle();

  writeExtraText = true;
  extraText = "DELPHES SIM";
  lumi_sqrtS = "35.9 fb^{-1} (13 TeV)";

  int iPeriod = 0;
  int iPos = 11;

  double Sigma_bb    = 0.175*Mass_a1;

  TCanvas *Canvas = new TCanvas("Canvas","histogram",800,800);
  Canvas->SetBottomMargin(0.2);
  Canvas->cd();

  auto pad = new TPad("pad","",0,0.0,1,1,0);
  pad->SetLeftMargin(0.1);
  pad->Draw();
  pad->cd();

  TFile *File = new TFile("../../ROOT_M"+TString::Itoa( (int)Mass_a1, 10 )+".root");
  TH1D *Hist = (TH1D*)File->Get("M_bb");
  Hist->SetTitle("");
  Hist->GetYaxis()->SetTitleOffset(1.5);

  int RebinSize = 1;
  if( Mass_a1 == 40 ) RebinSize = 2;
  else if( Mass_a1 == 60 ) RebinSize = 4;
  Hist->Rebin(RebinSize);
  Hist->GetXaxis()->SetRangeUser(Mass_a1-4.5*Sigma_bb,Mass_a1+4.5*Sigma_bb);
  Hist->SetMarkerStyle(21);
  Hist->SetMarkerSize(0.5);
  Hist->SetLineColor(kBlack);
  gStyle->SetOptStat(0000);
  gStyle->SetOptFit(0010);
  gStyle->SetStatX(0.95);
  gStyle->SetStatY(0.73);
  gStyle->SetStatW(0.14);                
  //gStyle->SetStatH(0.04);                
  gStyle->SetStatFontSize(0.02);
  Hist->SetStats(1);
  //Hist->SetStats(0);
  Hist->Draw("P");

  double PlotXLimit = 1.5; // 1.5;
  
  TF1 *FitFunction = new TF1("fit","gaus",Hist->GetMean()-Sigma_bb*PlotXLimit,Hist->GetMean()+Sigma_bb*PlotXLimit);
  FitFunction->SetParameters(Hist->GetMaximum()*0.95, Mass_a1, Sigma_bb);
//  FitFunction->FixParameter(1, Mass_a1);
//  FitFunction->SetParLimits(0, Hist->GetMaximum()*0.9, Hist->GetMaximum());
//  FitFunction->SetParLimits(2, 0.15*Mass_a1, 0.2*Mass_a1);

//  Hist->Fit(FitFunction, "B");
  Hist->Fit(FitFunction,"RLE");

  FitFunction->SetLineColor(kBlue);
  FitFunction->Draw("SAME");

  double tConst = FitFunction->GetParameter(0);
  double tMean = FitFunction->GetParameter(1);
  double tSigma = FitFunction->GetParameter(2);
  //double tAlpha1 = FitFunction->GetParameter(3);
  //double tExp1 = FitFunction->GetParameter(4);
  //double tAlpha2 = FitFunction->GetParameter(5);
  //double tExp2 = FitFunction->GetParameter(6);

  double tChi2 = FitFunction->GetChisquare();
  double tNdf = FitFunction->GetNDF();

  TLegend *leg;
  leg = new TLegend(0.71,0.78,0.93,0.90);
  leg->AddEntry(Hist,"m_{a_{1}} = "+TString::Itoa( (int)Mass_a1, 10 )+" GeV","lep");
  leg->AddEntry(FitFunction,"Gaus fit","f");
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->Draw();

  CMS_lumi( pad , iPeriod , iPos );

  Canvas->SaveAs("../../plots/bbMass_M(a1)="+TString::Itoa( (int)Mass_a1, 10 )+".png");
  Canvas->SaveAs("../../plots/bbMass_M(a1)="+TString::Itoa( (int)Mass_a1, 10 )+".pdf");

  cout<<endl<<endl;
  cout<<"+ --- histogram information --- +"<<endl;
  cout<<"mean : "<<Hist->GetMean()<<"  RMS : "<<Hist->GetRMS()<<endl<<endl;

  cout<<"+ --- fit result --- +"<<endl;
  cout<<"const : "<<tConst<<"  mean : "<<tMean<<"  sigma : "<<tSigma<<endl;
  cout<<"chi2 : "<<tChi2<<"  ndf : "<<tNdf<<"  chi2/dof : "<<tChi2/tNdf<<endl<<endl;

  //double sig = tSigma/Mass_a1;
  cout<<"Sigma/m(a1): "<<tSigma/Mass_a1<<endl;
  cout<<"Sigma/Mean: "<<tSigma/tMean<<endl;
//  cout<<"Sigma_bb    = [[ "+sig+" ]] * Mass_a1"<<endl;

}

