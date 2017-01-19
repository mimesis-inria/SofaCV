#include <sofa/helper/system/config.h>
#include "initplugin.h"

namespace sofa
{
namespace OR
{
namespace common
{
// Here are just several convenient functions to help user to know what contains
// the plugin

extern "C" {
SOFA_SOFAORCOMMONPLUGIN_API void initExternalModule();
SOFA_SOFAORCOMMONPLUGIN_API const char* getModuleName();
SOFA_SOFAORCOMMONPLUGIN_API const char* getModuleVersion();
SOFA_SOFAORCOMMONPLUGIN_API const char* getModuleLicense();
SOFA_SOFAORCOMMONPLUGIN_API const char* getModuleDescription();
SOFA_SOFAORCOMMONPLUGIN_API const char* getModuleComponentList();
}

void initExternalModule()
{
  static bool first = true;
  if (first)
  {
    first = false;
  }
}

const char* getModuleName() { return "SofaORCommon"; }
const char* getModuleVersion() { return "0.1"; }
const char* getModuleLicense() { return ""; }
const char* getModuleDescription()
{
  return "SofaORCommon plugin, containing data structures and debug/visu components";
}

const char* getModuleComponentList()
{
  return "FrameViewer;CalibLoader";
}

}  // namespace common

}  // namespace OR

}  // namespace sofa

////////// BEGIN CLASS LIST //////////

SOFA_LINK_CLASS(CalibLoader);
SOFA_LINK_CLASS(FrameViewer);
