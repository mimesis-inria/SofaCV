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
namespace cvmat
{
template <typename T>
inline void desserialize(std::istream &in, T *data)
{
}

template <>
inline void desserialize<unsigned char>(std::istream &in, unsigned char *buff)
{
  std::string tmpDelim;
  //  uchar *buff = (uchar *)(data);
  bool read = true;
  while (read)
  {
    in >> tmpDelim;
    if (tmpDelim == "[") continue;
    if (tmpDelim.find(']') != std::string::npos)
      break;
    else
    {
      *buff = atoi(tmpDelim.c_str());
      buff++;
    }
  }
}

template <>
inline void desserialize<char>(std::istream &in, char *buff)
{
  std::string tmpDelim;
  //  char *buff = (char *)(data);
  bool read = true;
  while (read)
  {
    in >> tmpDelim;
    if (tmpDelim == "[") continue;
    if (tmpDelim.find(']') != std::string::npos)
      break;
    else
    {
      *buff = atoi(tmpDelim.c_str());
      buff++;
    }
  }
}

template <>
inline void desserialize<unsigned short>(std::istream &in, unsigned short *buff)
{
  std::string tmpDelim;
  //  ushort *buff = (ushort *)(data);
  bool read = true;
  while (read)
  {
    in >> tmpDelim;
    if (tmpDelim == "[") continue;
    if (tmpDelim.find(']') != std::string::npos)
      break;
    else
    {
      *buff = atoi(tmpDelim.c_str());
      buff++;
    }
  }
}

template <>
inline void desserialize<short>(std::istream &in, short *buff)
{
  std::string tmpDelim;
  //  short *buff = (short *)(data);
  bool read = true;
  while (read)
  {
    in >> tmpDelim;
    if (tmpDelim == "[") continue;
    if (tmpDelim.find(']') != std::string::npos)
      break;
    else
    {
      *buff = atoi(tmpDelim.c_str());
      buff++;
    }
  }
}

template <>
inline void desserialize<int>(std::istream &in, int *buff)
{
  std::string tmpDelim;
  //  int *buff = (int *)(data);
  bool read = true;
  while (read)
  {
    in >> tmpDelim;
    if (tmpDelim == "[") continue;
    if (tmpDelim.find(']') != std::string::npos)
      break;
    else
    {
      *buff = atoi(tmpDelim.c_str());
      buff++;
    }
  }
}

template <>
inline void desserialize<float>(std::istream &in, float *buff)
{
  std::string tmpDelim;
  //  float *buff = (float *)(data);
  bool read = true;
  while (read)
  {
    in >> tmpDelim;
    if (tmpDelim == "[") continue;
    if (tmpDelim.find(']') != std::string::npos)
      break;
    else
    {
      *buff = atof(tmpDelim.c_str());
      buff++;
    }
  }
}

template <>
inline void desserialize<double>(std::istream &in, double *buff)
{
  std::string tmpDelim;
  //  double *buff = (double *)(data);
  bool read = true;
  while (read)
  {
    in >> tmpDelim;
    if (tmpDelim == "[") continue;
    if (tmpDelim.find(']') != std::string::npos)
      break;
    else
    {
      *buff = atof(tmpDelim.c_str());
      buff++;
    }
  }
}
}  // namespace cvmat

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

  template <class T>
  void desserialize(std::istream &in)
  {
    common::cvmat::desserialize<T>(in, (T *)data);
  }

  inline friend std::istream &operator>>(std::istream &in, cvMat &s)
  {
    int rows, cols, type;

    in >> rows >> cols >> type;
    s.create(rows, cols, type);
    switch (s.depth())
    {
      case CV_8U:
        s.desserialize<unsigned char>(in);
        break;
      case CV_8S:
        s.desserialize<char>(in);
        break;
      case CV_16U:
        s.desserialize<unsigned short>(in);
        break;
      case CV_16S:
        s.desserialize<short>(in);
        break;
      case CV_32S:
        s.desserialize<int>(in);
        break;
      case CV_32F:
        s.desserialize<float>(in);
        break;
      case CV_64F:
        s.desserialize<double>(in);
        break;
      default:
        msg_error("cvMat::desserialize()") << "Matrix depth type incorrect";
        break;
    }
    return in;
  }

  inline friend std::ostream &operator<<(std::ostream &out, const cvMat &s)
  {
    out << " " << s.rows << " " << s.cols << " " << s.type() << " ";
    out << *((cv::Mat *)&s);

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
  static std::string name() { return "cvMat"; }
};

}  // namespace defaulttype
}  // namespace sofa

#endif  // SOFA_OR_COMMON_CVMAT_H
