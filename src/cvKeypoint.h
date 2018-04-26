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

#ifndef SOFACV_CVKEYPOINT_H
#define SOFACV_CVKEYPOINT_H

#include "SofaCVPlugin.h"

#include <sofa/defaulttype/DataTypeInfo.h>

#include <opencv2/core.hpp>

#include <iostream>

namespace sofacv

{
/**
 * @brief The Keypoint class, Needed to override the stream operators for SOFA
 */
class SOFA_SOFACV_API cvKeypoint : public cv::KeyPoint
{
 public:
    cvKeypoint(const cv::KeyPoint& kp)
      : cv::KeyPoint(kp.pt, kp.size, kp.angle, kp.response, kp.octave,
                     kp.class_id)
  {
  }
  cvKeypoint() : cv::KeyPoint() {}
  cvKeypoint(cv::Point2f _pt, float _size, float _angle = -1,
             float _response = 0, int _octave = 0, int _class_id = -1)
      : cv::KeyPoint(_pt, _size, _angle, _response, _octave, _class_id)
  {
  }
  cvKeypoint(float x, float y, float _size, float _angle = -1,
             float _response = 0, int _octave = 0, int _class_id = -1)
      : cv::KeyPoint(x, y, _size, _angle, _response, _octave, _class_id)
  {
  }

    inline friend std::istream& operator>>(std::istream& in, cvKeypoint& s)
  {
		in >> s.pt.x >> s.pt.y >> s.size >> s.angle >> s.response >> s.octave >>
				s.class_id;
    return in;
  }

    inline friend std::ostream& operator<<(std::ostream& out, const cvKeypoint& s)
  {
		out << s.pt.x << ' ' << s.pt.y << ' ' << s.size << ' ' << s.angle << ' '
				<< s.response << ' ' << s.octave << ' ' << s.class_id << ' ';
    return out;
  }
};

}  // namespace sofacv

namespace sofa
{
namespace defaulttype
{
/**
 *  \brief Implementation of SOFA's DataType interface to pass cv::KeyPoint data
 * structures as sofa::Data
 */
template <>
struct DataTypeName<sofacv::cvKeypoint>
{
    static const char* name() { return "cvKeypoint"; }
};

}  // namespace defaulttype
}  // namespace sofa

#endif  // SOFACV_CVKEYPOINT_H
