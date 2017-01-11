#ifndef SOFA_OR_ACQUISITOR_FRAMEVIEWER_H
#define SOFA_OR_ACQUISITOR_FRAMEVIEWER_H

#include "initplugin.h"
#include "SofaORCommon/cvMat.h"

#include <sofa/core/DataEngine.h>

#include <opencv2/opencv.hpp>


namespace sofa
{
namespace OR
{
namespace common
{
class FrameViewer: public core::DataEngine
{
 public:
  SOFA_CLASS(FrameViewer, core::DataEngine);

 public:
  FrameViewer();
  virtual ~FrameViewer();

  void init();
  void update();
  void reinit();

  sofa::Data<common::cvMat> d_frame;
  std::string m_winID;
  void handleEvent(sofa::core::objectmodel::Event* event);
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_ACQUISITOR_FRAMEVIEWER_H
