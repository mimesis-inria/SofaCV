#ifndef SOFA_OR_ACQUISITOR_STEREOVIEWER_H
#define SOFA_OR_ACQUISITOR_STEREOVIEWER_H

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
class StereoViewer: public core::DataEngine
{
 public:
  SOFA_CLASS(StereoViewer, core::DataEngine);

 public:
  StereoViewer();
  virtual ~StereoViewer();

  void init();
  void update();
  void reinit();

  sofa::Data<common::cvMat> d_frameL;
  sofa::Data<common::cvMat> d_frameR;

  sofa::Data<defaulttype::Vec2f> d_pointsL;
  sofa::Data<defaulttype::Vec2f> d_pointsR;

  std::string m_winID;
  void handleEvent(sofa::core::objectmodel::Event* event);

private:
  common::cvMat m_stereoFrame;
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_ACQUISITOR_STEREOVIEWER_H
