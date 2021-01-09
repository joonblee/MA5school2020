#include "../../PlotHelper/tdrstyle.C"
#include "../../PlotHelper/CMS_lumi.C"

Double_t DCBFunc(Double_t *x, Double_t *par) {
  // 0(const), 1(mean), 2(sigma), 3(alpha1), 4(n1), 5(alpha2), 6(n2)
  Double_t t = (x[0]-par[1])/par[2];
  if (t>=-1*par[3] && t<=par[5]) {
    return par[0]*exp(-0.5*t*t);
  }
  else if (t<-1*par[3]) {
    Double_t A1 = pow(par[4]/fabs(par[3]),par[4])*exp(-1*par[3]*par[3]/2);
    Double_t B1 = par[4]/fabs(par[3])-fabs(par[3]);
    return par[0]*A1*pow(B1-t,-par[4]);
  }
  else if (t>par[5]) {
    Double_t A2 = pow(par[6]/fabs(par[5]),par[6])*exp(-par[5]*par[5]/2);
    Double_t B2 = par[6]/fabs(par[5])-fabs(par[5]);
    return par[0]*A2*pow(B2+t,-par[6]);
  }
  else {
    //cout << "ERROR evaluating range..." << endl;
    return 99;
  }
}


void HiggsRes(double Mass_a1 = 40.) {
  setTDRStyle();

  writeExtraText = true;
  extraText = "DELPHES SIM";
  lumi_sqrtS = "35.9 fb^{-1} (13 TeV)";

  int iPeriod = 0;
  int iPos = 11;
  
  TCanvas *Canvas = new TCanvas("Canvas","histogram",800,800);
  Canvas->SetBottomMargin(0.2);
  Canvas->cd();

  auto pad = new TPad("pad","",0,0.0,1,1,0);
  pad->SetLeftMargin(0.1);
  pad->Draw();
  pad->cd();

  TFile *File = new TFile("../../ROOT_M"+TString::Itoa( (int)Mass_a1, 10 )+".root");
  TH1D *Hist = (TH1D*)File->Get("M_mumubb");
  Hist->SetTitle("");
  Hist->GetYaxis()->SetTitleOffset(1.5);

  Hist->GetXaxis()->SetRangeUser(80.,170.);
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


  double Mass_h      = 125.18;
  double Sigma_h     = 10.6;

  double PlotXLimit = 1.5; 
  // Try 1.5, corresponding to 'bbRes.C' code. It gives width ~ 9.6, which is a very similar value of the detector-based estimating resonance, 9.3.
  // PlotXLimit = 2.; // Sigma_h = 10.4; -> (AN) Sigma_h = 10.6;

  TF1 *PreFit = new TF1("fit","gaus",Mass_h-Sigma_h*PlotXLimit,Mass_h+Sigma_h*PlotXLimit);
  PreFit->SetParameters(Hist->GetMaximum(), Mass_h, Sigma_h);
  Hist->Fit(PreFit, "R");
  double PreMean = PreFit->GetParameter(1);
 

  TF1 *FitFunction = new TF1("fit","gaus",PreMean-Sigma_h*PlotXLimit,PreMean+Sigma_h*PlotXLimit);
  //FitFunction->SetParameters(Hist->GetMaximum(), 126., Sigma_h);
  //FitFunction->SetParameters(Hist->GetMaximum(), PreMean, Sigma_h);
  //FitFunction->SetParLimits(0, Hist->GetMaximum()*0.95, Hist->GetMaximum());
  //FitFunction->FixParameter(1, 126.);
  //FitFunction->SetParLimits(1, 124. , Hist->GetMaximum());
  //FitFunction->SetParLimits(1, 123., 127.);

  //Hist->Fit(FitFunction, "BR");
  Hist->Fit(FitFunction, "R");

  FitFunction->SetLineColor(kBlue);
  FitFunction->Draw("SAME");


  double tConst = FitFunction->GetParameter(0);
  double tMean = FitFunction->GetParameter(1);
  double tSigma = FitFunction->GetParameter(2);

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

  Canvas->SaveAs("../../plots/HiggsMass_M(a1)="+TString::Itoa( (int)Mass_a1, 10 )+".png");
  Canvas->SaveAs("../../plots/HiggsMass_M(a1)="+TString::Itoa( (int)Mass_a1, 10 )+".pdf");


  cout<<endl<<endl;
  cout<<"+ --- histogram information --- +"<<endl;
  cout<<"mean : "<<Hist->GetMean()<<"  RMS : "<<Hist->GetRMS()<<endl<<endl;

  cout<<"+ --- fit result --- +"<<endl;
  cout<<"const : "<<tConst<<"  mean : "<<tMean<<"  sigma : "<<tSigma<<endl;
  cout<<"chi2 : "<<tChi2<<"  ndf : "<<tNdf<<"  chi2/dof : "<<tChi2/tNdf<<endl<<endl;

}

