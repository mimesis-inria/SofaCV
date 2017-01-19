#ifndef SOFA_OR_COMMON_CAMERACALIB_H
#define SOFA_OR_COMMON_CAMERACALIB_H

#include "cvMatUtils.h"

#include <sofa/defaulttype/Mat.h>
#include <sofa/helper/vector.h>

#include <iostream>
#include <opencv2/core/types.hpp>

namespace sofa
{
namespace OR
{
namespace common
{
class CameraCalib
{
 public:
  CameraCalib()
  {
  }
  CameraCalib(const CameraCalib &o)
      : distCoefs(o.distCoefs), cameraMatrix(o.cameraMatrix), error(o.error)
  {
  }

  CameraCalib(const cv::Mat_<double>& _cameraMatrix, const cv::Mat_<double>& _distCoefs,
              double _error)
  {
    common::matrix::cvMat2sofaMat<3, 3, double>(_cameraMatrix, cameraMatrix);
    common::matrix::cvMat2sofaVector(_distCoefs, distCoefs);
    error = _error;
  }
  inline friend std::istream &operator>>(std::istream &in, CameraCalib &s)
  {
    in >> s.distCoefs >> s.cameraMatrix >> s.error;
    return in;
  }

  inline friend std::ostream &operator<<(std::ostream &out,
                                         const CameraCalib &s)
  {
    out << s.distCoefs << s.cameraMatrix << s.error;
    return out;
  }

  helper::vector<double> distCoefs;
  defaulttype::Matrix3 cameraMatrix;
  double error;
};

}  // namespace processor
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_COMMON_CAMERACALIB_H
