#ifndef SOFA_OR_ACQUISITOR_FRAMEVIEWER_H
#define SOFA_OR_ACQUISITOR_FRAMEVIEWER_H

#include "initplugin.h"
#include "SofaORCommon/cvMat.h"

#include "SofaORCommon/ImplicitDataEngine.h"

#include <opencv2/opencv.hpp>


namespace sofa
{
namespace OR
{
namespace common
{
class FrameViewer: public ImplicitDataEngine
{
 public:
  SOFA_CLASS(FrameViewer, ImplicitDataEngine);

 public:
  FrameViewer();
  virtual ~FrameViewer();

  void init();
  void update();

  sofa::Data<common::cvMat> d_frame;
  std::string m_winID;
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_ACQUISITOR_FRAMEVIEWER_H
