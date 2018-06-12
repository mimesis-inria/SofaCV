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

#ifndef SOFACV_ACQUISITION_BASEGRABBER_H
#define SOFACV_ACQUISITION_BASEGRABBER_H

#include "SofaCVPlugin.h"
#include "ImplicitDataEngine.h"

namespace sofacv
{
namespace acquisition
{
/**
 * @brief The BaseGrabber interface
 *
 * All data grabbers should inherit this interface, as it provides a variation
 * to the ImplicitDataEngine's handleEvent function for data grabbers.
 */
class SOFA_SOFACV_API BaseGrabber : virtual public ImplicitDataEngine
{
 public:
  BaseGrabber() {}
  virtual ~BaseGrabber() {}

 protected:
  /// different handleEvent behavior for Data grabbers:
  /// checkData will always return false, as there is no input to this engine
  /// so:
  /// All grabbers should inherit a DataGrabber iface, itself deriving from
  /// ImplicitDataEngine
  /// A variant of handleEvent should be implemented there, that would look like
  /// this:
  virtual void handleEvent(sofa::core::objectmodel::Event* e)
  {
    if (sofa::simulation::AnimateBeginEvent::checkEventType(e))
    {
//      cleanDirty();
      update(); /// always call update() for grabbers. It's
                /// decided internally whether or not they should do something
      return;
    }
    ImplicitDataEngine::handleEvent(e);
  }
};

}  // namespace acquisition
}  // namespace sofacv
#endif  // SOFACV_ACQUISITION_BASEGRABBER_H
