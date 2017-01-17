#ifndef SOFA_OR_ACQUISITOR_CALIBLOADER_H
#define SOFA_OR_ACQUISITOR_CALIBLOADER_H

#include "SofaORCommon/CameraCalib.h"
#include "SofaORCommon/StereoCalib.h"
#include "SofaORCommon/cvMat.h"
#include "initplugin.h"

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/objectmodel/DataFileName.h>

#include <opencv2/core/core.hpp>

namespace sofa
{
namespace OR
{
namespace common
{
class CalibLoader : public core::objectmodel::BaseObject
{
 public:
  SOFA_CLASS(CalibLoader, core::objectmodel::BaseObject);

  CalibLoader();
  virtual ~CalibLoader();

  void init();
  void update();
  void reinit();

  sofa::core::objectmodel::DataFileName d_calibFile;
  Data<bool> d_isStereo;
  Data<common::CameraCalib> d_leftCalib;
  Data<common::CameraCalib> d_rightCalib;
  Data<common::StereoCalib> d_stereoCalib;
  Data<std::string> d_calibName;
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_ACQUISITOR_CALIBLOADER_H
