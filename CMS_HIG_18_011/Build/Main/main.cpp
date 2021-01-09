////////////////////////////////////////////////////////////////////////////////
//  
//  This file has been generated by MadAnalysis 5.
//  The MadAnalysis development team: <ma5team@iphc.cnrs.fr>
//                Eric Conte and Benjamin Fuks
//  Official website: <https://launchpad.net/madanalysis5>
//  
////////////////////////////////////////////////////////////////////////////////

// SampleHeader header
#include "SampleAnalyzer/Process/Core/SampleAnalyzer.h"
#include "SampleAnalyzer/User/Analyzer/analysisList.h"
using namespace MA5;

// -----------------------------------------------------------------------
// Info function
// -----------------------------------------------------------------------
int Info(SampleAnalyzer& manager)
{
  INFO << "BEGIN " << __FILE__ << endmsg;
  manager.AnalyzerList().Print();
  INFO << "END " << __FILE__ << endmsg;
  return 0;
}
// -----------------------------------------------------------------------
// main program
// -----------------------------------------------------------------------
int main(int argc, char *argv[])
{
  // Creating a manager
  SampleAnalyzer manager;
  BuildUserTable(manager.AnalyzerList());

  // Identifying --info argument
  if (argc==2)
  {
    std::string arg=argv[1];
    if (arg=="--info") return Info(manager);
  }

  // ---------------------------------------------------
  //                    INITIALIZATION
  // ---------------------------------------------------
  INFO << "    * Initializing all components" << endmsg;

  // Initializing the manager
  if (!manager.Initialize(argc,argv,"pdg.ma5")) return 1;

  // Creating data format for storing data
  EventFormat myEvent;
  std::vector<SampleFormat> mySamples;

  // Getting pointer to the analyzer
  std::map<std::string, std::string> parametersA1;
  AnalyzerBase* analyzer1 = 
      manager.InitializeAnalyzer("CMS_HIG_18_011","user.saf",parametersA1);
  if (analyzer1==0) return 1;

  // Post initialization (creates the new output directory structure)
  if(!manager.PostInitialize()) return 1;

  // ---------------------------------------------------
  //                      EXECUTION
  // ---------------------------------------------------
  INFO << "    * Running over files ..." << endmsg;

  // Loop over files
  while(1)
  {
    // Opening input file
    mySamples.push_back(SampleFormat());
    SampleFormat& mySample=mySamples.back();
    StatusCode::Type result1 = manager.NextFile(mySample);
    if (result1!=StatusCode::KEEP)
    {
      if (result1==StatusCode::SKIP) continue;
      else if (result1==StatusCode::FAILURE) {mySamples.pop_back(); break;}
    }
    
    // Loop over events
    while(1)
    {
      StatusCode::Type result2 = manager.NextEvent(mySample,myEvent);
      if (result2!=StatusCode::KEEP)
      {
        if (result2==StatusCode::SKIP) continue;
        else if (result2==StatusCode::FAILURE) break;
      }
          manager.UpdateProgressBar();
      if (!analyzer1->Execute(mySample,myEvent)) continue;
    }
  }

  // ---------------------------------------------------
  //                     FINALIZATION
  // ---------------------------------------------------
  INFO << "    * Finalizing all components ..." << endmsg;

  // Finalizing all components
  manager.Finalize(mySamples,myEvent);
  return 0;
}
