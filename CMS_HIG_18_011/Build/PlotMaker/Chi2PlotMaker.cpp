#include "../PlotHelper/tdrstyle.C"
#include "../PlotHelper/CMS_lumi.C"

void Chi2PlotMaker(int Mass = 40) {
  setTDRStyle();

  writeExtraText = true;
  extraText = "DELPHES SIM";
  lumi_13TeV = "35.9 fb^{-1}";
  lumi_sqrtS = "13 TeV";

  int iPeriod = 0;
  int iPos = 11;

  TString str_Mass = TString::Itoa(Mass, 10);

  //TFile *File = new TFile("../ROOT_M"+str_Mass+".root");
  //TH1D *Hist = (TH1D*)File->Get("Chi2");

  TH1D *Hist = new TH1D("Hist","Hist",25,0,50);
  //vector<double> bins={2.608456e+03,8.101681e+02,4.786982e+02,3.320172e+02,2.403908e+02,1.867433e+02,1.638440e+02,1.315483e+02,1.049854e+02,8.318498e+01,6.913044e+01,6.013287e+01,5.172705e+01,4.802039e+01,3.962701e+01,3.552436e+01,3.487100e+01,3.760022e+01,3.723289e+01,3.489248e+01,2.728171e+01,2.776128e+01,2.717604e+01,2.403902e+01,2.434581e+01,2.243056e+03};
  vector<double> bins={2.708456e+03,8.001681e+02,4.786982e+02,3.320172e+02,2.403908e+02,1.867433e+02,1.638440e+02,1.315483e+02,1.049854e+02,8.318498e+01,6.913044e+01,6.013287e+01,5.172705e+01,4.802039e+01,3.962701e+01,3.552436e+01,3.487100e+01,3.760022e+01,3.723289e+01,3.489248e+01,2.728171e+01,2.776128e+01,2.717604e+01,2.403902e+01,2.434581e+01,2.243056e+03};
  for(int i=0; i<25; i++) {
    Hist->SetBinContent(i+1,bins[i]);
  }
  Hist->SetLineColor(kBlue);
  //Hist->SetLineWidth(2);

  TH1D *CMSHist = new TH1D("CMS", "CMS", 25, 0, 50);
  if(Mass==40) {
    vector<double> contents={ 0.5347732874999904, 0.1410678453419371, 0.07622134331543279, 0.045558722990768324, 0.0347026644400291, 0.023415097956326562, 0.01595959026517172, 0.01612872835300607, 0.011331405481364463, 0.010144653711666191, 0.00890058892469175, 0.007808729054908714, 0.00805262192548077, 0.0077374729788328295, 0.006788399475719037, 0.006328070845662859, 0.006726454093048688, 0.005901391521962751, 0.0036360831693276903, 0.008078253204913276, 0.004277474821634864, 0.004929278949379385, 0.003498320935775036, 0.004835145937243088, 0.002534507741593958};
    for(int i=0; i<25; i++) {
      CMSHist->SetBinContent(i+1,contents[i]);
    }
    CMSHist->SetLineColor(kRed);
    //CMSHist->SetLineWidth(2);
  }


  TCanvas *Canvas = new TCanvas("Canvas","",1000,1000);
  Canvas->cd();
  Canvas->SetLogy();

  Canvas->SetLeftMargin(0.15);
  Canvas->SetBottomMargin(0.13);

  double SumWeights = 0;
  for(unsigned i=0; i<Hist->GetNbinsX(); i++) {
    SumWeights += Hist->GetBinContent(i);
  }
  Hist->Scale(1./SumWeights);

  //Hist->GetXaxis()->SetTitle("#font[7]{#Chi^{2}_{h} + #Chi^{2}_{b}}");
  Hist->GetXaxis()->SetTitle("#chi^{2}_{h} + #chi^{2}_{b}");
  Hist->GetYaxis()->SetTitle("a. u.");
  Hist->GetYaxis()->SetRangeUser(0.001,1.);

  Hist->SetStats(0);
  Hist->Draw("Hist");
  if(Mass==40) CMSHist->Draw("HIST SAME");

  //Hist->SetTitle("Recasting [CMS-HIG-18-011]");
  Hist->SetTitle("");

  double leg_xmin=0.60, leg_xmax=0.88, leg_ymin=0.80, leg_ymax=0.90;
  if(Mass==40) leg_ymin=0.70;
  TLegend *leg;
  leg = new TLegend(leg_xmin,leg_ymin,leg_xmax,leg_ymax);
  leg->AddEntry(Hist,"MA5 recasting","f");
  if(Mass==40) leg->AddEntry(CMSHist,"CMS-HIG-18-011","f");
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->Draw();

  CMS_lumi( Canvas , iPeriod , iPos );

  Canvas->SaveAs("../plots/Chi2_M"+str_Mass+".png");
  Canvas->SaveAs("../plots/Chi2_M"+str_Mass+".pdf");
}
