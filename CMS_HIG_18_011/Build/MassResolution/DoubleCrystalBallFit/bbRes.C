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




void bbRes(double Mass_a1 = 40.) {
  double Sigma_bb    = 0.145*Mass_a1;

  TCanvas *Canvas = new TCanvas("Canvas","histogram",800,800);
  Canvas->SetBottomMargin(0.2);
  Canvas->cd();

  auto pad1 = new TPad("pad1","",0,0.0,1,1,0);
  pad1->SetLeftMargin(0.1);
  pad1->Draw();
  pad1->cd();

  TFile *File = new TFile("../../ROOT_M"+TString::Itoa( (int)Mass_a1, 10 )+".root");
  TH1D *Hist = (TH1D*)File->Get("M_bb");
  int RebinSize = 1;
  if( Mass_a1 == 40 ) RebinSize = 2;
  else if( Mass_a1 == 60 ) RebinSize = 4;
  Hist->Rebin(RebinSize);
  Hist->GetXaxis()->SetRangeUser(Mass_a1-4.5*Sigma_bb,Mass_a1+4.5*Sigma_bb);
  Hist->SetMarkerStyle(21);
  Hist->SetMarkerSize(0.5);
  Hist->SetLineColor(kBlack);
  Hist->SetStats(0);
  Hist->Draw("P");



  double unc1 = 0.03;
  double unc2 = 0.3; double unc3 = -0.05; // mean
  double unc4 = 0.2; double unc5 = 0.2; // sigma
  double PlotXLimit = 4.;
  if( Mass_a1 == 40. ) PlotXLimit = 3.;

  TF1 *FitFunction = new TF1("fit",DCBFunc,Mass_a1-Sigma_bb*PlotXLimit,Mass_a1+Sigma_bb*PlotXLimit,7);
  FitFunction->SetParameters(Hist->GetMaximum()*0.95, Mass_a1*0.9, Sigma_bb, 1., 10., 1., 10.);
  FitFunction->SetParNames("Const","Mean","Sigma","alpha1","Exp1","alpha2","Exp2");

  // FitFunction->SetParLimits(0, 0.19, Hist->GetMaximum()*1.05); // Maximum
  FitFunction->SetParLimits(0, Hist->GetMaximum()*(1.0-unc1), Hist->GetMaximum()*(1.+unc1)); // Maximum
  // FitFunction->SetParLimits(1, Mass_a1*(1.0-unc2), 18.5); // mean
  FitFunction->SetParLimits(1, Mass_a1*(1.0-unc2), Mass_a1*(1.0+unc3)); // mean
  FitFunction->SetParLimits(2, Sigma_bb*(1.0-unc4), Sigma_bb*(1.0+unc5)); // sigma
  FitFunction->SetParLimits(3, .5, 3.0);
  FitFunction->SetParLimits(4, .01, 9999.0);
  FitFunction->SetParLimits(5, .5, 3.0);
  FitFunction->SetParLimits(6, .01, 9999.0);



  Hist->Fit(FitFunction, "RLE");

  FitFunction->SetLineColor(kBlue);
  FitFunction->Draw("SAME");


  double tConst = FitFunction->GetParameter(0);
  double tMean = FitFunction->GetParameter(1);
  double tSigma = FitFunction->GetParameter(2);
  double tAlpha1 = FitFunction->GetParameter(3);
  double tExp1 = FitFunction->GetParameter(4);
  double tAlpha2 = FitFunction->GetParameter(5);
  double tExp2 = FitFunction->GetParameter(6);

  double tChi2 = FitFunction->GetChisquare();
  double tNdf = FitFunction->GetNDF();


  Canvas->SaveAs("../../plots/bbMass_M(a1)="+TString::Itoa( (int)Mass_a1, 10 )+".png");

  cout<<endl<<endl;
  cout<<"+ --- fit result --- +"<<endl;
  cout<<"const : "<<tConst<<"  mean : "<<tMean<<"  sigma : "<<tSigma<<endl;
  cout<<"alpha1 : "<<tAlpha1<<"  exp1 : "<<tExp1<<"  alpha2 : "<<tAlpha2<<"  exp2 : "<<tExp2<<endl;
  cout<<"chi2 : "<<tChi2<<"  ndf : "<<tNdf<<"  chi2/dof : "<<tChi2/tNdf<<endl<<endl;

  //double sig = tSigma/Mass_a1;
  cout<<"Sigma/m(a1): "<<tSigma/Mass_a1<<endl;
  cout<<"Sigma/Mean: "<<tSigma/tMean<<endl;
//  cout<<"Sigma_bb    = [[ "+sig+" ]] * Mass_a1"<<endl;

}

