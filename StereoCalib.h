#ifndef SOFA_OR_COMMON_STEREOCALIB_H
#define SOFA_OR_COMMON_STEREOCALIB_H

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
class StereoCalib
{
 public:
  StereoCalib() {}
  StereoCalib(const StereoCalib &o)
      : R(o.R), T(o.T), F(o.F), error(o.error)
  {
  }

  StereoCalib(cv::Mat_<double> &_R, cv::Mat_<double> &_T, cv::Mat_<double> &_F,
              double _error)
  {
    common::matrix::cvMat2sofaMat(_R, R);
    common::matrix::cvMat2sofaVector(_T, T);
    common::matrix::cvMat2sofaMat(_F, F);
    error = _error;
  }
  inline friend std::istream &operator>>(std::istream &in, StereoCalib &s)
  {
    in >> s.R >> s.T >> s.F >> s.error;
    return in;
  }

  inline friend std::ostream &operator<<(std::ostream &out,
                                         const StereoCalib &s)
  {
    out << s.R << s.T << s.F << s.error;
    return out;
  }

  defaulttype::Matrix3 R;
  defaulttype::Vector3 T;
  defaulttype::Matrix3 F;
  double error;
};

}  // namespace processor
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_COMMON_STEREOCALIB_H
