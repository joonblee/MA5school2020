#include "SampleAnalyzer/User/Analyzer/CMS_HIG_18_011.h"
#include "SampleAnalyzer/Process/Analyzer/AnalyzerManager.h"
#include "SampleAnalyzer/Commons/Service/LogStream.h"

// -----------------------------------------------------------------------------
// BuildTable
// -----------------------------------------------------------------------------
void BuildUserTable(MA5::AnalyzerManager& manager)
{
  using namespace MA5;
  manager.Add("CMS_HIG_18_011",new CMS_HIG_18_011);
}
