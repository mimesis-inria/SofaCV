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

#ifndef SOFA_OR_COMMON_CVMAT_H
#define SOFA_OR_COMMON_CVMAT_H

#include "initplugin.h"

#include <sofa/defaulttype/DataTypeInfo.h>
#include <iostream>
#include <opencv2/core.hpp>

namespace sofaor
{
namespace common
{
/**
 * @brief The cvMat class, Needed to override the stream operators for SOFA
 */
class SOFA_SOFAORCOMMON_API cvMat : public cv::Mat
{
 public:
  cvMat() : cv::Mat() {}
  cvMat(int rows, int cols, int type) : cv::Mat(rows, cols, type) {}
  cvMat(cv::Size size, int type) : cv::Mat(size, type) {}
  cvMat(int rows, int cols, int type, const cv::Scalar &s)
      : cv::Mat(rows, cols, type, s)
  {
  }
  cvMat(cv::Size size, int type, const cv::Scalar &s) : cv::Mat(size, type, s)
  {
  }
  cvMat(int ndims, const int *sizes, int type) : cv::Mat(ndims, sizes, type) {}
  cvMat(int ndims, const int *sizes, int type, const cv::Scalar &s)
      : cv::Mat(ndims, sizes, type, s)
  {
  }
  cvMat(const cv::Mat &m) : cv::Mat(m) {}
  cvMat(int rows, int cols, int type, void *data, size_t step = AUTO_STEP)
      : cv::Mat(rows, cols, type, data, step)
  {
  }

  cvMat(cv::Size size, int type, void *data, size_t step = AUTO_STEP)
      : cv::Mat(size, type, data, step)
  {
  }
  cvMat(int ndims, const int *sizes, int type, void *data,
        const size_t *steps = 0)
      : cv::Mat(ndims, sizes, type, data, steps)
  {
  }

  cvMat(const cv::Mat &m, const cv::Range &rowRange,
        const cv::Range &colRange = cv::Range::all())
      : cv::Mat(m, rowRange, colRange)
  {
  }

  cvMat(const cv::Mat &m, const cv::Rect &roi) : cv::Mat(m, roi) {}
  cvMat(const cv::Mat &m, const cv::Range *ranges) : cv::Mat(m, ranges) {}
  inline friend std::istream &operator>>(std::istream &in, cvMat &s)
  {
    int cols, rows, type;
    void *ptr;
    in >> cols >> rows >> type >> ptr;
    cvMat image(rows, cols, type, ptr);
    image.copyTo(s);
    return in;
  }

  inline friend std::ostream &operator<<(std::ostream &out, const cvMat &s)
  {
    out << s.cols << " " << s.rows << " " << s.type() << " " << (void *)s.data;
    out << " ";

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
 *  \brief Implementation of SOFA's DataType interface to pass cv::Mat data
 * structures as sofa::Data
 */
template <>
struct DataTypeName<sofaor::common::cvMat>
{
  static const char *name() { return "cvMat"; }
};

}  // namespace defaulttype
}  // namespace sofa

#endif  // SOFA_OR_COMMON_CVMAT_H
