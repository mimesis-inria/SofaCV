/******************************************************************************
 *       SOFAOR, SOFA plugin for the Operating Room, development version       *
 *                        (c) 2017 INRIA, MIMESIS Team                         *
 *                                                                             *
 * This program is a free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation; either version 1.0 of the License, or (at     *
 * your option) any later version.                                             *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
 * for more details.                                                           *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.        *
 *******************************************************************************
 * Authors: Bruno Marques and external contributors (see Authors.txt)          *
 *                                                                             *
 * Contact information: contact-mimesis@inria.fr                               *
 ******************************************************************************/

#include "initplugin.h"
#include <sofa/config.h>
#include <sofa/helper/system/config.h>

#ifdef QT_PLUGIN
#include <QApplication>
#include <QDebug>
#include <QQuickPaintedItem>

const int versionMajor = 1;
const int versionMinor = 0;

static void initResources() { Q_INIT_RESOURCE(SofaORCommon_qml); }
#endif  // QT_PLUGIN

#ifdef SOFA_HAVE_SOFAPYTHON
#include <SofaPython/PythonFactory.h>
#include "python/Binding_cvMatData.h"
#endif

namespace sofaor
{
/**
 * \brief Utility classes for SOFA-OR
 */
namespace common
{
}  // namespace common
}  // namespace sofaor

namespace sofa
{
namespace component
{
// Here are just several convenient functions to help user to know what contains
// the plugin
extern "C" {
SOFA_SOFAORCOMMON_API void initExternalModule();
SOFA_SOFAORCOMMON_API const char* getModuleName();
SOFA_SOFAORCOMMON_API const char* getModuleVersion();
SOFA_SOFAORCOMMON_API const char* getModuleLicense();
SOFA_SOFAORCOMMON_API const char* getModuleDescription();
SOFA_SOFAORCOMMON_API const char* getModuleComponentList();
}

void initExternalModule()
{
  static bool first = true;
  if (first)
  {
    first = false;
#ifdef QT_PLUGIN
    initResources();
#endif  // QT_PLUGIN
#ifdef SOFA_HAVE_SOFAPYTHON
    if (PythonFactory::s_sofaPythonModule)
    {
      simulation::PythonEnvironment::gil lock(__func__);

      // adding new bindings for Data<cvMat>
      SP_ADD_CLASS_IN_FACTORY(cvMatData, sofa::Data<sofaor::common::cvMat>)
    }
#endif
  }
}

const char* getModuleName() { return "SofaORCommon"; }
const char* getModuleVersion() { return "0.1"; }
const char* getModuleLicense() { return ""; }
const char* getModuleDescription()
{
  return "SofaORCommon plugin, containing data structures and utility "
         "functions";
}

const char* getModuleComponentList() { return ""; }

}  // namespace common
}  // namespace sofaor
