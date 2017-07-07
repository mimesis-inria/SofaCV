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

#ifndef SOFAORCOMMONPLUGIN_H
#define SOFAORCOMMONPLUGIN_H

#include <sofa/helper/system/config.h>

#define SOFAORCOMMONPLUGIN_MAJOR_VERSION \
  $ { SOFAORCOMMONPLUGIN_MAJOR_VERSION }
#define SOFAORCOMMONPLUGIN_MINOR_VERSION \
  $ { SOFAORCOMMONPLUGIN_MINOR_VERSION }
#ifdef SOFA_BUILD_SOFAORCOMMONPLUGIN
#define SOFA_TARGET SofaORCommonPlugin
#define SOFA_SOFAORCOMMONPLUGIN_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#define SOFA_SOFAORCOMMONPLUGIN_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#endif  // SOFA_BUILD_SOFAORCOMMONPLUGIN
