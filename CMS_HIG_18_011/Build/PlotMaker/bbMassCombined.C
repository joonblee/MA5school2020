#include "../PlotHelper/tdrstyle.C"
#include "../PlotHelper/CMS_lumi.C"

void bbMassCombined() {
  setTDRStyle();

  writeExtraText = true;
  extraText = "DELPHES SIM";
  lumi_13TeV = "35.9 fb^{-1}";
  lumi_sqrtS = "13 TeV";

  int iPeriod = 0;
  int iPos = 11;

  vector<TFile*> File;
  File.push_back( new TFile("../ROOT_M20.root") );
  File.push_back( new TFile("../ROOT_M40.root") );
  File.push_back( new TFile("../ROOT_M60.root") );

  vector<TH1D*> Hist;
  for( unsigned i=0; i<File.size(); i++ ) {
    Hist.push_back( (TH1D*)File[i]->Get("M_bb") );
    Hist[i]->SetTitle("");
		Hist[i]->GetYaxis()->SetTitleOffset(1.);
    Hist[i]->SetStats(0);
    Hist[i]->SetLineWidth(2.0);
  }

  TCanvas *Canvas = new TCanvas("Canvas","",1000,1000);
  Canvas->cd();
  Canvas->SetLogy();

  Hist[0]->GetXaxis()->SetTitle("m_{bb} (GeV)");
  Hist[0]->GetYaxis()->SetTitle("Events / 5 GeV");
  Hist[0]->GetYaxis()->SetRangeUser(0.1,20.);

  Hist[0]->SetLineColor(9);
  Hist[1]->SetLineColor(41);
  Hist[2]->SetLineColor(6);

  Hist[0]->Draw("Hist");
  Hist[1]->Draw("Hist SAME");
  Hist[2]->Draw("Hist SAME");

  TLegend *leg;
  leg = new TLegend(0.65,0.70,0.92,0.90);
  leg->AddEntry(Hist[0],"m_{a_{1}} = 20 GeV","l");
  leg->AddEntry(Hist[1],"m_{a_{1}} = 40 GeV","l");
  leg->AddEntry(Hist[2],"m_{a_{1}} = 60 GeV","l");
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->Draw();

  CMS_lumi( Canvas , iPeriod , iPos );

  Canvas->SaveAs("../plots/bbMassCombined.png");
  Canvas->SaveAs("../plots/bbMassCombined.pdf");
}

