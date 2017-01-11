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


} // namespace image
} // namespace common
} // namespace OR
} // namespace sofa
