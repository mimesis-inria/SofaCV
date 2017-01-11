#ifndef SOFA_OR_COMMON_CVMAT_H
#define SOFA_OR_COMMON_CVMAT_H

#include <opencv2/core.hpp>
#include <iostream>

namespace sofa
{
namespace OR
{
namespace common
{
class cvMat : public cv::Mat
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
      cvMat image;
      in.read(reinterpret_cast<char*>(&image), sizeof(cvMat*));
      image.copyTo(s);
    return in;
  }

  inline friend std::ostream &operator<<(std::ostream &out, const cvMat &s)
  {
    out.write(reinterpret_cast<const char*>(&s), sizeof(cvMat*));
    return out;
  }
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_COMMON_CVMAT_H
