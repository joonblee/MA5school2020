#ifndef analysis_CMS_HIG_18_011_h
#define analysis_CMS_HIG_18_011_h
//#include "SampleAnalyzer/Interfaces/root/RootMainHeaders.h"
#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

namespace MA5
{
  class CMS_HIG_18_011 : public AnalyzerBase
  {
    INIT_ANALYSIS(CMS_HIG_18_011,"CMS_HIG_18_011")
  
    public:
    virtual bool Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters);
    virtual void Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files);
    virtual bool Execute(SampleFormat& sample, const EventFormat& event);
  
    private:
    double pfIsolation(const EventFormat& event, const RecLeptonFormat* Muon); 
    bool MuonVeto(const RecJetFormat* Jet, std::vector<const RecLeptonFormat*> Muons);
    double LooseBTagEff(const RecJetFormat* Jet);
    double TightBTagEff(const RecJetFormat* Jet);
  };
}

#endif

