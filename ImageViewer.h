#ifndef SOFA_OR_ACQUISITOR_IMAGEVIEWER_H
#define SOFA_OR_ACQUISITOR_IMAGEVIEWER_H

#include "initplugin.h"

#include <sofa/core/DataEngine.h>

#include <SofaORCommon/Image.h>
#include <opencv2/opencv.hpp>


namespace sofa
{
namespace OR
{
namespace common
{
class ImageViewer: public core::DataEngine
{
 public:
  SOFA_CLASS(ImageViewer, core::DataEngine);

 public:
  ImageViewer();
  virtual ~ImageViewer();

  void init();
  void update();
  void reinit();

  sofa::Data<common::Image> d_image;
  std::string m_winID;
  void handleEvent(sofa::core::objectmodel::Event* event);
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_ACQUISITOR_IMAGEVIEWER_H
