#include "ImageViewer.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/AnimateBeginEvent.h>

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
  f_listening.setValue(true);
  static int i = 0;
  m_winID = std::string("ImageViewer");
  m_winID += std::to_string(i);
  cv::namedWindow(m_winID, CV_WINDOW_AUTOSIZE);
  i++;
}

ImageViewer::~ImageViewer() {}
void ImageViewer::init()
{
  addInput(&d_image);
  d_image.setDirtyValue();
  if (!d_image.getValue().empty())
  {
    cv::imshow(m_winID, d_image.getValue());
    cv::waitKey(1);
  }
}

void ImageViewer::update()
{
  if (d_image.isDirty())
  {
    d_image.update();
    d_image.cleanDirty();
    if (!d_image.getValue().empty())
    {
      cv::imshow(m_winID, d_image.getValue());
      cv::waitKey(1);
    }
  }
}

void ImageViewer::reinit() { update(); }
void ImageViewer::handleEvent(sofa::core::objectmodel::Event* e)
{
  if (sofa::simulation::AnimateBeginEvent::checkEventType(e)) this->update();
}

}  // namespace common
}  // namespace OR
}  // namespace sofa
