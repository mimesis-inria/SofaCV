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

#ifndef SOFA_OR_COMMON_FROMPYTHON_H
#define SOFA_OR_COMMON_FROMPYTHON_H

#include "initplugin.h"

#include <sstream>
#include "SofaORCommon/ImplicitDataEngine.h"
#include "SofaORCommon/cvMat.h"

namespace sofaor
{
namespace common
{
/**
 * @brief The ProjectPoints class
 *
 * Projects a 2D point cloud in 3D or vice-versa using a linked CameraSettings
 * component
 */
class FromPython : public common::ImplicitDataEngine
{
 public:
  SOFA_CLASS(FromPython, common::ImplicitDataEngine);

  FromPython()
      : d_in(initData(&d_in, "img", "image buffer as sent  from python")),
        d_out(initData(&d_out, "img_out", ""))
  {
  }

  ~FromPython() {}
  void init()
  {
    addInput(&d_in);
    addOutput(&d_out);
    update();
  }

  void update()
  {
    std::stringstream ss;
    ss << d_in.getValue();
    cvMat& img = *d_out.beginEdit();
    ss >> img;
    d_out.endEdit();
  }

  virtual void handleEvent(sofa::core::objectmodel::Event* e)
  {
    if (sofa::core::objectmodel::IdleEvent::checkEventType(e) ||
        sofa::simulation::AnimateBeginEvent::checkEventType(e))
    {
      update();
      setDirtyOutputs();
    }
  }

  sofa::Data<std::string>
      d_in;                 ///< [INPUT] string buffer as received from Python
  sofa::Data<cvMat> d_out;  ///< [OUTPUT] output image
};

}  // namespace common
}  // namespace sofaor
#endif  // SOFA_OR_COMMON_FROMPYTHON_H
