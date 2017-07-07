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

#ifndef SOFA_OR_COMMON_CVDMATCH_H
#define SOFA_OR_COMMON_CVDMATCH_H

#include <sofa/defaulttype/DataTypeInfo.h>
#include <iostream>
#include <opencv2/core/types.hpp>

namespace sofaor
{
namespace common
{
/**
 * @brief The cvDMatch class, Needed to override the stream operators for SOFA
 */
class cvDMatch : public cv::DMatch
{
 public:
  cvDMatch()
  {
    distance = -1.0f;
    imgIdx = -1;
    queryIdx = -1;
    trainIdx = -1;
  }
  cvDMatch(const cv::DMatch& o)
  {
    distance = o.distance;
    imgIdx = o.imgIdx;
    queryIdx = o.queryIdx;
    trainIdx = o.trainIdx;
  }
  cvDMatch(int _queryIdx, int _trainIdx, float _distance)
      : cv::DMatch(_queryIdx, _trainIdx, _distance)
  {
  }
  cvDMatch(int _queryIdx, int _trainIdx, int _imgIdx, float _distance)
      : cv::DMatch(_queryIdx, _trainIdx, _imgIdx, _distance)
  {
  }

  inline friend std::istream& operator>>(std::istream& in, cvDMatch& s)
  {
    in >> s.distance >> s.imgIdx >> s.queryIdx >> s.trainIdx;
    return in;
  }

  inline friend std::ostream& operator<<(std::ostream& out, const cvDMatch& s)
  {
		out << s.distance << ' ' << s.imgIdx << ' ' << s.queryIdx << ' '
        << s.trainIdx << ' ';
    return out;
  }
};

}  // namespace common
}  // namespace sofaor

namespace sofa
{
namespace defaulttype
{
/**
 *  \brief Implementation of SOFA's DataType interface to pass cv::DMatch data
 * structures as sofa::Data
 */
template <>
struct DataTypeName<sofaor::common::cvDMatch>
{
	static const char* name() { return "cvDMatch"; }
};

}  // namespace defaulttype
}  // namespace sofa

#endif  // SOFA_OR_COMMON_CVDMATCH_H
