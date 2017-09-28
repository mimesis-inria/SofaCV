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
  static const char *Name() { return "cvMat"; }

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
<<<<<<< Updated upstream
    int cols, rows, type;
    void *ptr;
    in >> cols >> rows >> type >> ptr;
    cvMat image(rows, cols, type, ptr);
    image.copyTo(s);
=======
    size_t rows, cols, depth, channels;
    in >> rows >> cols >> depth >> channels;

    //    char separator;
    //    in.read(&separator, 1);

    //    cvMat image(rows, cols, CV_MAKETYPE(depth, channels));
    //    if (image.cols * image.rows)
    //    {
    //      in.read((char *)image.data, image.total() * image.elemSize());
    //      image.copyTo(s);
    //    }
>>>>>>> Stashed changes
    return in;
  }

  inline friend std::ostream &operator<<(std::ostream &out, const cvMat &s)
  {
<<<<<<< Updated upstream
    out << s.cols << " " << s.rows << " " << s.type() << " " << (void *)s.data;
    out << " ";
=======
    out << s.rows << " " << s.cols << " " << s.depth() << " " << s.channels()
        /*<< ";"*/;
    //    out.write((char *)s.data, s.total() * s.elemSize());
>>>>>>> Stashed changes

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

template <class TDataType>
struct cvMatTypeInfo
{
  typedef TDataType DataType;
  typedef DataType BaseType;
  typedef DataType ValueType;
  typedef long long ConvType;
  typedef ScalarTypeInfo<TDataType> BaseTypeInfo;
  typedef ScalarTypeInfo<TDataType> ValueTypeInfo;

  enum
  {
    ValidInfo = BaseTypeInfo::ValidInfo
  };  ///< 1 if this type has valid infos
  enum
  {
    FixedSize = 1
  };  ///< 1 if this type has a fixed size  -> always 1 Image
  enum
  {
    ZeroConstructor = 0
  };  ///< 1 if the constructor is equivalent to setting memory to 0  -> I guess
      ///< so, a default Image is initialzed with nothing
  enum
  {
    SimpleCopy = 0
  };  ///< 1 if copying the data can be done with a memcpy
  enum
  {
    SimpleLayout = 0
  };  ///< 1 if the layout in memory is simply N values of the same base type
  enum
  {
    Integer = 0
  };  ///< 1 if this type uses integer values
  enum
  {
    Scalar = 0
  };  ///< 1 if this type uses scalar values
  enum
  {
    Text = 0
  };  ///< 1 if this type uses text values
  enum
  {
    CopyOnWrite = 1
  };  ///< 1 if this type uses copy-on-write -> it seems to be THE important
      ///< option not to perform too many copies
  enum
  {
    Container = 0
  };  ///< 1 if this type is a container

  enum
  {
    Size = 1
  };  ///< largest known fixed size for this type, as returned by size()

  static size_t size() { return 1; }
  static size_t byteSize() { return 1; }

  static size_t size(const DataType & /*data*/) { return 1; }

  static bool setSize(DataType & /*data*/, size_t /*size*/) { return false; }

  template <typename T>
  static void getValue(const DataType & /*data*/, size_t /*index*/,
                       T & /*value*/)
  {
    return;
  }

  template <typename T>
  static void setValue(DataType & /*data*/, size_t /*index*/,
                       const T & /*value*/)
  {
    return;
  }

  static void getValueString(const DataType &data, size_t index,
                             std::string &value)
  {
    if (index != 0) return;
    std::ostringstream o;
    o << data;
    value = o.str();
  }

  static void setValueString(DataType &data, size_t index,
                             const std::string &value)
  {
    if (index != 0) return;
    std::istringstream i(value);
    i >> data;
  }

  static const void *getValuePtr(const DataType &) { return NULL; }

  static void *getValuePtr(DataType &) { return NULL; }
};

template <>
struct DataTypeInfo<sofaor::common::cvMat>
    : public cvMatTypeInfo<sofaor::common::cvMat>
{
  static std::string name()
  {
    return "cvMat";
  }
};

}  // namespace defaulttype
}  // namespace sofa

#endif  // SOFA_OR_COMMON_CVMAT_H
