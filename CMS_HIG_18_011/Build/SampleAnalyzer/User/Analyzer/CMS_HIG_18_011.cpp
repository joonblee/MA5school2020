#include "SampleAnalyzer/User/Analyzer/CMS_HIG_18_011.h"

using namespace MA5;
using namespace std;

// -----------------------------------------------------------------------------
// Initialize
// function called one time at the beginning of the analysis
// -----------------------------------------------------------------------------
bool CMS_HIG_18_011::Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters)
{
  Manager()->AddRegionSelection("mumubb");

  Manager()->AddCut("preselection");
  Manager()->AddCut("finalselection");

  //std::string Chi2[] = {"Chi2"};
  //Manager()->AddHisto("Chi2",25,0,50,Chi2);
  Manager()->AddHisto("Chi2",25,0,50);

  return true;
}

// -----------------------------------------------------------------------------
// Finalize
// function called one time at the end of the analysis
// -----------------------------------------------------------------------------
void CMS_HIG_18_011::Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files)
{
}

// -----------------------------------------------------------------------------
// Execute
// function called each time one event is read
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Directories of header files
//
// SampleFormat :: MA5_DIR//tools/SampleAnalyzer/Commons/DataFormat/SampleFormat.h
// EventFormat :: MA5_DIR//tools/SampleAnalyzer/Commons/DataFormat/EventFormat.h
// All other Rec, MC headers are in MA5_DIR//tools/SampleAnalyzer/Commons/DataFormat/REC_OR_MC_Header.h
//
// MA_Something like MALorentzVector are in MA5_DIR//tools/SampleAnalyzer/Commons/Vector
//
// Isolation, EFlow, Tagging information are in MA5_DIR//tools/SampleAnalyzer/Commons/Service
//
// Delphes cards are in MA5_DIR//tools/SampleAnalyzer/Interfaces/delphes/
// -----------------------------------------------------------------------------

bool CMS_HIG_18_011::Execute(SampleFormat& sample, const EventFormat& event) {
  // --- event weight --- //
  double weight=1;
  if(Configuration().IsNoEventWeight()) weight=1.;
  else if(event.mc()->weight()!=0.) weight=event.mc()->weight();
  else
  {
    WARNING << "Found one event with a zero weight. Skipping..." << endmsg;
    return false;
  }

  //the loop start
  if(event.rec()==0) { return true;}

  // --- mumubb selection --- //
  std::vector<const RecLeptonFormat*> MuMuCandidate;
  for(unsigned int iMuon = 0; iMuon < event.rec()->muons().size(); iMuon++) {
    const RecLeptonFormat* LeadingMuon = &(event.rec()->muons()[iMuon]);

    if (!(LeadingMuon->pt() > 20)) continue;
    if (!(fabs(LeadingMuon->eta()) < 2.4)) continue;
    if (!(pfIsolation(event, LeadingMuon) < .15)) continue;

    for(unsigned int jMuon = iMuon+1; jMuon < event.rec()->muons().size(); jMuon++) {
      const RecLeptonFormat* SubleadingMuon = &(event.rec()->muons()[jMuon]);

      if (!(SubleadingMuon->pt() > 9)) continue;
      if (!(fabs(SubleadingMuon->eta()) < 2.4)) continue;
      if (!(pfIsolation(event, SubleadingMuon) < .15)) continue;

      if (!(LeadingMuon->charge()*SubleadingMuon->charge() < 0)) continue;

      double mass = (LeadingMuon->momentum()+SubleadingMuon->momentum()).M();
      if (!( 19.5 < mass && mass < 63.5 )) continue;

      MuMuCandidate.push_back(LeadingMuon);
      MuMuCandidate.push_back(SubleadingMuon);

      break;
    }
    if( MuMuCandidate.size() == 2 ) break;
  }
  bool MuonPairSelection = (MuMuCandidate.size() == 2);

  std::vector<const RecJetFormat*> bbCandidate;
  if( MuonPairSelection ) {
    for(unsigned int iJet = 0; iJet<event.rec()->jets().size(); iJet++) {
      const RecJetFormat* LeadingJet = &(event.rec()->jets()[iJet]);
      
      if (!(LeadingJet->pt() > 20.)) continue;
      if (!(fabs(LeadingJet->eta()) < 2.4)) continue;
      if (!(MuonVeto(LeadingJet, MuMuCandidate))) continue;
      if (!(LeadingJet->btag())) continue;
  
      for(unsigned int jJet = iJet+1; jJet<event.rec()->jets().size(); jJet++) {
        const RecJetFormat* SubleadingJet = &(event.rec()->jets()[jJet]);
  
        if (!(SubleadingJet->pt() > 15)) continue;
        if (!(fabs(SubleadingJet->eta()) < 2.4)) continue;
        if (!(MuonVeto(SubleadingJet, MuMuCandidate))) continue;
        if (!(SubleadingJet->btag())) continue;
  
        bbCandidate.push_back(LeadingJet);
        bbCandidate.push_back(SubleadingJet);
  
        break;
      }
      if( bbCandidate.size() == 2 ) break;
    } 
  } 
  bool JetPairSelection = (bbCandidate.size() == 2);


  // --- event reweight --- //
  if( JetPairSelection ) {
    weight *= (.81/.84)*(.81/.84);
    weight *= (1 - (1 - TightBTagEff( bbCandidate[0] )/LooseBTagEff( bbCandidate[0] ))*(1 - TightBTagEff( bbCandidate[1] )/LooseBTagEff( bbCandidate[1] )));
  }

  Manager()->InitializeForNewEvent(weight);


  if(!Manager()->ApplyCut(MuonPairSelection&&JetPairSelection, "preselection")) return true;


  // --- final selection --- //
  MALorentzVector MET_p4 = event.rec()->MET().momentum();
  double MET_Pt = MET_p4.Pt();

  bool METCut = (MET_Pt < 60);

  double Mass_bb     = (bbCandidate[0]->momentum()+bbCandidate[1]->momentum()).M();
  double Mass_MuMu   = (MuMuCandidate[0]->momentum()+MuMuCandidate[1]->momentum()).M();
  double Mass_MuMubb = (MuMuCandidate[0]->momentum()+MuMuCandidate[1]->momentum()+bbCandidate[0]->momentum()+bbCandidate[1]->momentum()).M();
  double Mass_h      = 125.18;
  // --- Mass resolution --- //
  // = From Gaussian fit = //
  //double Sigma_bb    = 0.173*Mass_MuMu; // 1.5*sig fit 
  //double Sigma_h     = 9.66;          // 1.5*sig fit
  double Sigma_bb    = 0.173*Mass_MuMu; // 1.5*sig fit 
  double Sigma_h     = 9.66;          // 1.5*sig fit
  // ----------------------- //

  double Chi_bb = (Mass_bb - Mass_MuMu)/Sigma_bb;
  double Chi_h  = (Mass_MuMubb - Mass_h)/Sigma_h;
  double Chi2_   = pow(Chi_bb,2) + pow(Chi_h,2);

  Manager()->FillHisto("Chi2",Chi2_);

  bool Chi2Cut = (Chi2_ < 5);

  if(!Manager()->ApplyCut(METCut&&Chi2Cut, "finalselection")) return true;

  return true;
}


double CMS_HIG_18_011::pfIsolation(const EventFormat& event, const RecLeptonFormat* Muon) {
  double Isolation = 0.;

  MALorentzVector Muon_p4 = Muon->momentum();

  double ChargedHadronIso = PHYSICS->Isol->eflow->relIsolation(Muon, event.rec(), .4, 1., IsolationEFlow::TRACK_COMPONENT);
  double NeutralHadronIso = PHYSICS->Isol->eflow->relIsolation(Muon, event.rec(), .4, 1., IsolationEFlow::NEUTRAL_COMPONENT);
  double PhotonIso = PHYSICS->Isol->eflow->relIsolation(Muon, event.rec(), .4, 1., IsolationEFlow::PHOTON_COMPONENT);
  Isolation = ChargedHadronIso + NeutralHadronIso + PhotonIso;

  return Isolation;
}

bool CMS_HIG_18_011::MuonVeto(const RecJetFormat* Jet, std::vector<const RecLeptonFormat*> Muons) {
  for(unsigned iMuon = 0; iMuon < Muons.size(); iMuon++) {
    if( Jet->momentum().DeltaR( Muons[iMuon]->momentum() ) < 0.5 )
      return false;
  }
  return true;
}

double CMS_HIG_18_011::LooseBTagEff(const RecJetFormat* Jet) {
  double out;
  double pT = Jet->pt();
  if( pT < 160 )
    out = 0.4344 + 0.02069*pT - 0.0004429*pow(pT,2) + 5.137*pow(10,-6)*pow(pT,3) - 3.406*pow(10,-8)*pow(pT,4) + 1.285*pow(10,-10)*pow(pT,5) - 2.559*pow(10,-13)*pow(pT,6) + 2.084*pow(10,-16)*pow(pT,7);
  else if( pT < 300 )
    out = 0.714 + 0.002617*pT - 1.656*pow(10,-5)*pow(pT,2) + 4.767*pow(10,-8)*pow(pT,3) - 6.431*pow(10,-11)*pow(pT,4) + 3.287*pow(10,-14)*pow(pT,5);
  else out = 0.872 - 6.885*pow(10,-5)*pT + 4.34*pow(10,-8)*pT*pT;

  out *= .81/.84;

  return out;
}

double CMS_HIG_18_011::TightBTagEff(const RecJetFormat* Jet) {
  double out;
  double pT = Jet->pt();
  if( pT < 50 )
    out = -0.033 + 0.0225*pT - 0.00035*pow(pT,2) + 2.586*pow(10,-6)*pow(pT,3) - 9.096*pow(10,-9)*pow(pT,4) + 1.212*pow(10,-11)*pow(pT,5);
  else if( pT < 160 )
    out = 0.169 + 0.013*pT - 0.00019*pow(pT,2) + 1.373*pow(10,-6)*pow(pT,3) - 4.923*pow(10,-9)*pow(pT,4) + 6.87*pow(10,-12)*pow(pT,5);
  else out = 0.62 - 0.00083*pT + 4.3078*pow(10,-7)*pow(pT,2);

  out *= .41/.50;

  return out;
}

