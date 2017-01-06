#include "ImageViewer.h"

#include <sofa/core/ObjectFactory.h>

namespace sofa
{
namespace OR
{
namespace common
{
SOFA_DECL_CLASS(ImageViewer)

int ImageViewerClass =
    core::RegisterObject(
        "debug component to visualize images using OpenCV highgui")
        .add<ImageViewer>();

ImageViewer::ImageViewer()
    : d_image(initData(&d_image, "image", "image to display in opencv window"))
{
  static int i = 0;
  m_winID = std::string("ImageViewer");
  m_winID += std::to_string(i);
  cv::namedWindow(m_winID, CV_WINDOW_NORMAL);
  i++;
}

ImageViewer::~ImageViewer() {}
void ImageViewer::init() {}
void ImageViewer::update()
{
  cv::imshow(m_winID, d_image.getValue());
  cv::waitKey(1);
}
void ImageViewer::reinit() {}
}  // namespace common
}  // namespace OR
}  // namespace sofa
