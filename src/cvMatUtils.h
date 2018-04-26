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

#ifndef SOFACV_CVMATUTILS_H
#define SOFACV_CVMATUTILS_H

#include <sofa/defaulttype/Mat.h>

#include <opencv2/core.hpp>

namespace sofacv
{
namespace image
{
/// (c.f. see http://www.stereo3d.com/formats.htm)
enum VideoMode
{
  MONO = 0,
  STEREO_INTERLEAVED = 1,
  STEREO_TOP_BOTTOM = 2,
  STEREO_SIDE_BY_SIDE = 3,
  STEREO_ROTATED_SIDE_BY_SIDE = 4
};

}  // namespace image

/**
 * \brief utility conversions from OpenCV vector / matrix data structures to
 * SOFA & vice versa
 */
namespace matrix
{
template <int L, int C, class T = double>
void cvMat2sofaMat(const cv::Mat_<T>& src, sofa::defaulttype::Mat<L, C, T>& dst)
{
  assert(src.cols == C && src.rows == L);
    std::memcpy(dst.ptr(), src.ptr(), src.total() * src.elemSize());
}
template <int L, int C, class T = double>
void cvMat2sofaMat(const cv::Mat& src, sofa::defaulttype::Mat<L, C, T>& dst)
{
    assert(src.cols == C && src.rows == L);
    std::memcpy(dst.ptr(), src.ptr(), src.total() * src.elemSize());
}

template <int L, int C, class T = double>
void sofaMat2cvMat(const sofa::defaulttype::Mat<L, C, T>& src, cv::Mat_<T>& dst)
{
  dst = cv::Mat_<T>(L, C);
  std::memcpy(dst.ptr(), src.ptr(), L * C * sizeof(T));
}
template <int L, int C, class T = double>
void sofaMat2cvMat(const sofa::defaulttype::Mat<L, C, T>& src, cv::Mat& dst)
{
    dst = cv::Mat_<T>(L, C);
    std::memcpy(dst.ptr(), src.ptr(), L * C * sizeof(T));
}

template <int N, class T>
void cvMat2sofaVector(const cv::Mat_<T>& src, sofa::defaulttype::Vec<N, T>& dst)
{
  assert(src.total() == N);
  std::memcpy(dst.ptr(), src.data, src.total() * src.elemSize());
}
template <int N, class T>
void cvMat2sofaVector(const cv::Mat& src, sofa::defaulttype::Vec<N, T>& dst)
{
    assert(src.total() == N);
    std::memcpy(dst.ptr(), src.data, src.total() * src.elemSize());
}

template <class T>
void cvMat2sofaVector(const cv::Mat_<T>& src, sofa::helper::vector<T>& dst)
{
  dst.reserve(src.total());
  const T* ptr = reinterpret_cast<T*>(src.data);
  for (size_t i = 0; i < src.total(); ++i)
  {
    dst.push_back(ptr[i]);
  }
}
template <class T>
void cvMat2sofaVector(const cv::Mat& src, sofa::helper::vector<T>& dst)
{
    dst.reserve(src.total());
    const T* ptr = reinterpret_cast<T*>(src.data);
    for (size_t i = 0; i < src.total(); ++i)
    {
        dst.push_back(ptr[i]);
    }
}

template <class T>
void sofaVector2cvMat(const sofa::helper::vector<T>& src, cv::Mat_<T>& dst)
{
    dst = cv::Mat_<T>(src.size(), 1);
    std::memcpy(dst.ptr(), src.data(), src.size() * sizeof(T));
}

template <class T>
void sofaVector2cvMat(const sofa::helper::vector<T>& src, cv::Mat& dst)
{
    dst = cv::Mat_<T>(src.size(), 1);
    std::memcpy(dst.ptr(), src.data(), src.size() * sizeof(T));
}

template <int N, class T>
void sofaVector2cvMat(const sofa::defaulttype::Vec<N, T>& src, cv::Mat_<T>& dst)
{
    dst = cv::Mat_<T>(N, 1);
    std::memcpy(dst.ptr(), src.data(), N * sizeof(T));
}
template <int N, class T>
void sofaVector2cvMat(const sofa::defaulttype::Vec<N, T>& src, cv::Mat& dst)
{
    dst = cv::Mat_<T>(N, 1);
    std::memcpy(dst.ptr(), src.data(), N * sizeof(T));
}

}  // namespace matrix

}  // namespace sofacv

#endif  // SOFACV_CVMATUTILS_H
