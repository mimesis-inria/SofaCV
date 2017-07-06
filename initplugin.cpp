#include <sofa/helper/system/config.h>
#include "initplugin.h"

namespace sofaor
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
	return "SofaORCommon plugin, containing data structures and utility functions";
}

const char* getModuleComponentList()
{
	return "";
}

}  // namespace common
}  // namespace sofaor
