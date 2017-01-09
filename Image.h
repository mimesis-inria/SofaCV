#ifndef SOFA_OR_COMMON_FRAME_H
#define SOFA_OR_COMMON_FRAME_H

#include <SofaORCommon/Data.h>
#include <opencv2/core.hpp>
#include <iostream>

namespace sofa
{
namespace OR
{
namespace common
{
enum VideoMode
{
  MONO = 0,
  STEREO_INTERLEAVED = 1,
  STEREO_TOP_BOTTOM = 2,
  STEREO_SIDE_BY_SIDE = 3,
  STEREO_ROTATED_SIDE_BY_SIDE = 4
};

class Image : public cv::Mat, public sofa::OR::common::Data
{
 public:
  Image() : cv::Mat() {}
  Image(int rows, int cols, int type) : cv::Mat(rows, cols, type) {}
  Image(cv::Size size, int type) : cv::Mat(size, type) {}
  Image(int rows, int cols, int type, const cv::Scalar &s)
      : cv::Mat(rows, cols, type, s)
  {
  }
  Image(cv::Size size, int type, const cv::Scalar &s) : cv::Mat(size, type, s)
  {
  }
  Image(int ndims, const int *sizes, int type) : cv::Mat(ndims, sizes, type) {}
  Image(int ndims, const int *sizes, int type, const cv::Scalar &s)
      : cv::Mat(ndims, sizes, type, s)
  {
  }
  Image(const cv::Mat &m) : cv::Mat(m) {}
  Image(int rows, int cols, int type, void *data, size_t step = AUTO_STEP)
      : cv::Mat(rows, cols, type, data, step)
  {
  }

  Image(cv::Size size, int type, void *data, size_t step = AUTO_STEP)
      : cv::Mat(size, type, data, step)
  {
  }
  Image(int ndims, const int *sizes, int type, void *data,
        const size_t *steps = 0)
      : cv::Mat(ndims, sizes, type, data, steps)
  {
  }

  Image(const cv::Mat &m, const cv::Range &rowRange,
        const cv::Range &colRange = cv::Range::all())
      : cv::Mat(m, rowRange, colRange)
  {
  }

  Image(const cv::Mat &m, const cv::Rect &roi) : cv::Mat(m, roi) {}
  Image(const cv::Mat &m, const cv::Range *ranges) : cv::Mat(m, ranges) {}

  inline friend std::istream &operator>>(std::istream &in, Image &s)
  {
      Image image;
      in.read(reinterpret_cast<char*>(&image), sizeof(Image*));
      image.copyTo(s);
    return in;
  }

  inline friend std::ostream &operator<<(std::ostream &out, const Image &s)
  {
    out.write(reinterpret_cast<const char*>(&s), sizeof(Image*));
    return out;
  }
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_COMMON_FRAME_H
