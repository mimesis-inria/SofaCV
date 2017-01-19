#ifndef SOFA_OR_ACQUISITOR_CALIBLOADER_H
#define SOFA_OR_ACQUISITOR_CALIBLOADER_H

#include "SofaORCommon/CameraCalib.h"
#include "SofaORCommon/StereoCalib.h"
#include "SofaORCommon/cvMat.h"
#include "initplugin.h"

#include <sofa/core/DataEngine.h>
#include <sofa/core/objectmodel/DataFileName.h>
#include <sofa/helper/OptionsGroup.h>

#include <opencv2/core/core.hpp>

#include <map>

namespace sofa
{
namespace OR
{
namespace common
{
class CalibLoader : public core::DataEngine
{
  struct CalibData
  {
    CalibData() {}
    CalibData(const common::CameraCalib& _camL,
              const common::CameraCalib& _camR,
              const common::StereoCalib& _extrinsics)
        : camL(_camL), camR(_camR), extrinsics(_extrinsics)
    {
    }
    common::CameraCalib camL;
    common::CameraCalib camR;
    common::StereoCalib extrinsics;
  };

 public:
  SOFA_CLASS(CalibLoader, core::DataEngine);

  CalibLoader();
  virtual ~CalibLoader();

  void init();
  void update();
  void reinit();

  sofa::core::objectmodel::DataFileName d_calibFolder;
  Data<bool> d_isStereo;
  Data<common::CameraCalib> d_leftCalib;
  Data<common::CameraCalib> d_rightCalib;
  Data<common::StereoCalib> d_stereoCalib;
  Data<helper::OptionsGroup> d_calibNames;

  std::map<std::string, CalibData> m_calibs;

  void handleEvent(sofa::core::objectmodel::Event* event);

 private:
  void load(const std::string& calibfile);
  bool canLoad(const std::string& calibfile) const;
  void setCurrentCalib(const std::string& calibName);
  std::string getPathToCalibs();
  void getAllCalibFiles(std::vector<std::string>& calibFiles);
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_ACQUISITOR_CALIBLOADER_H
