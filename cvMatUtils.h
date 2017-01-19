#ifndef SOFA_OR_COMMON_CVMATUTILS_H
#define SOFA_OR_COMMON_CVMATUTILS_H

#include <sofa/defaulttype/Mat.h>

#include <igtl/igtlImageMessage.h>
#include <opencv2/opencv.hpp>

namespace sofa
{
namespace OR
{
namespace common
{
namespace image
{
enum VideoMode
{
  MONO = 0,
  STEREO_INTERLEAVED = 1,
  STEREO_TOP_BOTTOM = 2,
  STEREO_SIDE_BY_SIDE = 3,
  STEREO_ROTATED_SIDE_BY_SIDE = 4
};

//#ifdef SOFAOR_ENABLE_OPENIGTLINK

inline int getOpenCVDepth(int depth)
{
  switch (depth)
  {
    case CV_8S:
      return igtl::ImageMessage::TYPE_INT8;
    case CV_8U:
      return igtl::ImageMessage::TYPE_UINT8;
    case CV_16S:
      return igtl::ImageMessage::TYPE_INT16;
    case CV_16U:
      return igtl::ImageMessage::TYPE_UINT16;
    case CV_32S:
      return igtl::ImageMessage::TYPE_INT32;
    case CV_32F:
      return igtl::ImageMessage::TYPE_FLOAT32;
    case CV_64F:
      return igtl::ImageMessage::TYPE_FLOAT64;
    default:
      return 0;
  }
}
//#endif  // SOFAOR_ENABLE_OPENIGTLINK

}  // namespace image

namespace matrix
{
template <int L, int C, class T = float>
void cvMat2sofaMat(const cv::Mat_<T>& src, defaulttype::Mat<L, C, T>& dst)
{
  assert(src.cols == C && src.rows == L);
  std::memcpy(dst.ptr(), src.data, src.total() * src.elemSize());
}

template <int L, int C, class T = float>
void sofaMat2cvMat(const defaulttype::Mat<L, C, T>& src, cv::Mat_<T>& dst)
{
  dst = cv::Mat_<T>(L, C);
  std::memcpy(dst.ptr(), src.ptr(), L*C*sizeof(T));
}

template <int N, class T>
void cvMat2sofaVector(const cv::Mat_<T>& src, defaulttype::Vec<N, T>& dst)
{
  assert(src.total() == N);
  std::memcpy(dst.ptr(), src.data, src.total() * src.elemSize());
}

template <class T>
void cvMat2sofaVector(const cv::Mat_<T>& src, helper::vector<T>& dst)
{
  dst.reserve(src.total());
  const T* ptr = reinterpret_cast<T*>(src.data);
  for (size_t i = 0; i < src.total(); ++i)
  {
    dst.push_back(ptr[i]);
  }
}

}  // namespace matrix

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_COMMON_CVMATUTILS_H
