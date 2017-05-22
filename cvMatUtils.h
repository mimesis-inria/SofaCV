#ifndef SOFA_OR_COMMON_CVMATUTILS_H
#define SOFA_OR_COMMON_CVMATUTILS_H

#include <sofa/defaulttype/Mat.h>

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

}  // namespace image

namespace matrix
{
template <int L, int C, class T = double>
void cvMat2sofaMat(const cv::Mat_<T>& src, defaulttype::Mat<L, C, T>& dst)
{
  assert(src.cols == C && src.rows == L);
	std::memcpy(dst.ptr(), src.ptr(), src.total() * src.elemSize());
}
template <int L, int C, class T = double>
void cvMat2sofaMat(const cv::Mat& src, defaulttype::Mat<L, C, T>& dst)
{
	assert(src.cols == C && src.rows == L);
	std::memcpy(dst.ptr(), src.ptr(), src.total() * src.elemSize());
}

template <int L, int C, class T = double>
void sofaMat2cvMat(const defaulttype::Mat<L, C, T>& src, cv::Mat_<T>& dst)
{
  dst = cv::Mat_<T>(L, C);
  std::memcpy(dst.ptr(), src.ptr(), L * C * sizeof(T));
}
template <int L, int C, class T = double>
void sofaMat2cvMat(const defaulttype::Mat<L, C, T>& src, cv::Mat& dst)
{
	dst = cv::Mat_<T>(L, C);
	std::memcpy(dst.ptr(), src.ptr(), L * C * sizeof(T));
}

template <int N, class T>
void cvMat2sofaVector(const cv::Mat_<T>& src, defaulttype::Vec<N, T>& dst)
{
  assert(src.total() == N);
  std::memcpy(dst.ptr(), src.data, src.total() * src.elemSize());
}
template <int N, class T>
void cvMat2sofaVector(const cv::Mat& src, defaulttype::Vec<N, T>& dst)
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
template <class T>
void cvMat2sofaVector(const cv::Mat& src, helper::vector<T>& dst)
{
	dst.reserve(src.total());
	const T* ptr = reinterpret_cast<T*>(src.data);
	for (size_t i = 0; i < src.total(); ++i)
	{
		dst.push_back(ptr[i]);
	}
}

template <class T>
void sofaVector2cvMat(const helper::vector<T>& src, cv::Mat_<T>& dst)
{
	dst = cv::Mat_<T>(src.size(), 1);
	std::memcpy(dst.ptr(), src.data(), src.size() * sizeof(T));
}

template <class T>
void sofaVector2cvMat(const helper::vector<T>& src, cv::Mat& dst)
{
	dst = cv::Mat_<T>(src.size(), 1);
	std::memcpy(dst.ptr(), src.data(), src.size() * sizeof(T));
}

template <int N, class T>
void sofaVector2cvMat(const defaulttype::Vec<N, T>& src, cv::Mat_<T>& dst)
{
	dst = cv::Mat_<T>(N, 1);
	std::memcpy(dst.ptr(), src.data(), N * sizeof(T));
}
template <int N, class T>
void sofaVector2cvMat(const defaulttype::Vec<N, T>& src, cv::Mat& dst)
{
	dst = cv::Mat_<T>(N, 1);
	std::memcpy(dst.ptr(), src.data(), N * sizeof(T));
}

}  // namespace matrix

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_COMMON_CVMATUTILS_H
