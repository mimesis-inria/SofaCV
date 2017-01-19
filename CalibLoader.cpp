#include "CalibLoader.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/system/FileSystem.h>
#include <sofa/helper/system/SetDirectory.h>
#include <sofa/simulation/AnimateBeginEvent.h>

#include <cstring>
#include <fstream>
#include <string>

#include <opencv2/core/persistence.hpp>

namespace sofa
{
namespace OR
{
namespace common
{
SOFA_DECL_CLASS(CalibLoader)

int CalibLoaderClass =
    core::RegisterObject("Mono / stereo Camera calibration settings loader")
        .add<CalibLoader>();

CalibLoader::CalibLoader()
    : d_calibFolder(initData(&d_calibFolder, "calib_dir",
                             "directory in which calibrations are stored")),
      d_isStereo(initData(&d_isStereo, "is_stereo",
                          "set to true if the given calib file has stereo data",
                          true, true)),
      d_leftCalib(initData(&d_leftCalib, "left_calib",
                           "Left camera calib data (or default if mono)", true,
                           true)),
      d_rightCalib(initData(&d_rightCalib, "right_calib",
                            "Right camera calib data", true, true)),
      d_stereoCalib(initData(&d_stereoCalib, "stereo_calib",
                             "extrinsic parameters (Rt, E and F matrices)",
                             true, true)),
      d_calibNames(initData(&d_calibNames, "calib_name",
                            "name of the calib settings currently used"))

{
  f_listening.setValue(true);
}

CalibLoader::~CalibLoader() {}
bool CalibLoader::canLoad(const std::string& calibfile) const
{
  if (calibfile == "")
  {
    msg_error("CalibLoader::canLoad()")
        << "Error: CalibLoader was given an empty calibName";
    return false;
  }

  // -- Check if file exist:
  const char* filename = calibfile.c_str();
  std::string sfilename(filename);

  if (!sofa::helper::system::DataRepository.findFile(sfilename))
  {
    msg_error("CalibLoader::canLoad()") << "Error: File '" << calibfile
                                        << "' not found. ";
    return false;
  }

  std::ifstream f(filename);

  // -- Check if file is readable:
  if (!f.good())
  {
    msg_error("CalibLoader::canLoad()") << "Error: Cannot read file '"
                                        << calibfile << "'.";
    return false;
  }
  f.close();
  return true;
}

void CalibLoader::setCurrentCalib(const std::string& calibName)
{
  d_isStereo.setValue(false);
  auto it = m_calibs.find(calibName);
  if (it != m_calibs.end())
  {
    helper::OptionsGroup* t = d_calibNames.beginWriteOnly();
    t->setSelectedItem(it->first);
    d_calibNames.endEdit();
    d_leftCalib.setValue(it->second.camL);
    if (!it->second.camR.cameraMatrix.empty())
    {
      d_isStereo.setValue(true);
      d_rightCalib.setValue(it->second.camR);
      d_stereoCalib.setValue(it->second.extrinsics);
    }
  }
}

void CalibLoader::load(const std::string& filename)
{
  cv::FileStorage* fs = NULL;
  try
  {
    fs = new cv::FileStorage(filename, cv::FileStorage::READ);
  }
  catch (cv::Exception& e)
  {
    msg_error("CalibLoader::load()")
        << "cv::FileStorage::open(): File is not a valid XML / YAML file\n"
        << e.what();
    return;
  }

  std::string calibName;
  cv::Mat cameraMatrixL;
  cv::Mat cameraMatrixR;
  cv::Mat distVecL;
  cv::Mat distVecR;
  double errorL;
  double errorR;
  cv::Mat R;
  cv::Mat T;
  cv::Mat F;
  double totalError;

  calibName = helper::system::SetDirectory::GetFileNameWithoutExtension(
      filename.c_str());

  cv::read((*fs)["cam_mats_left"], cameraMatrixL, cv::Mat());
  cv::read((*fs)["cam_mats_right"], cameraMatrixR, cv::Mat());
  cv::read((*fs)["error_left"], errorL, -1.0);
  cv::read((*fs)["error_right"], errorR, -1.0);
  cv::read((*fs)["dist_coefs_left"], distVecL, cv::Mat());
  cv::read((*fs)["dist_coefs_right"], distVecR, cv::Mat());
  cv::read((*fs)["rot_mat"], R, cv::Mat());
  cv::read((*fs)["trans_vec"], T, cv::Mat());
  cv::read((*fs)["f_mat"], F, cv::Mat());
  cv::read((*fs)["avg_error"], totalError, -1.0);

  m_calibs[calibName] =
      CalibData(common::CameraCalib(cameraMatrixL, distVecL, errorL),
                common::CameraCalib(cameraMatrixR, distVecR, errorR),
                common::StereoCalib(R, T, F, totalError));
  if (calibName == d_calibNames.getValue().getSelectedItem())
    setCurrentCalib(calibName);
  fs->release();
}

std::string CalibLoader::getPathToCalibs()
{
  std::string currentDir = helper::system::SetDirectory::GetCurrentDir();
  // If exists, add calibFolder to current dir
  if (d_calibFolder.getValue() != "")
  {
    std::string path = d_calibFolder.getValue();

    if (!path.empty())
      currentDir = helper::system::SetDirectory::GetRelativeFromDir(
          path.c_str(), currentDir.c_str());
  }
  return currentDir;
}

void CalibLoader::getAllCalibFiles(std::vector<std::string>& calibFiles)
{
  // Retrieve all calib files (files ending in ".yml" in calibFolder)
  helper::system::FileSystem::listDirectory(getPathToCalibs(), calibFiles,
                                            "yml");
}

void CalibLoader::init()
{
  addOutput(&d_isStereo);
  addOutput(&d_calibNames);
  addOutput(&d_leftCalib);
  addOutput(&d_rightCalib);
  addOutput(&d_stereoCalib);
  reinit();
  setDirtyOutputs();
}

void CalibLoader::update() {}
void CalibLoader::reinit()
{
  std::string calibname = d_calibNames.getValue().getSelectedItem();

  m_calibs.clear();
  helper::OptionsGroup* t = d_calibNames.beginEdit();
  t->setNames(0);
  d_calibNames.endEdit();

  std::string currentDir = getPathToCalibs();
  std::vector<std::string> calibFiles;
  getAllCalibFiles(calibFiles);

  if (calibFiles.empty())
  {
    helper::OptionsGroup* t = d_calibNames.beginEdit();
    t->setNames(1, "NO_CALIB");
    m_calibs["NO_CALIB"];
    setCurrentCalib("NO_CALIB");
    d_calibNames.endEdit();
  }
  else
  {
    helper::OptionsGroup* t = d_calibNames.beginEdit();
    t->setNbItems(unsigned(calibFiles.size()));
    unsigned i = 0;
    for (std::string& s : calibFiles)
    {
      t->setItemName(
          i++,
          helper::system::SetDirectory::GetFileNameWithoutExtension(s.c_str()));
    }
    (calibname != "") ? (t->setSelectedItem(calibname))
                      : t->setSelectedItemToDefault();
    d_calibNames.endEdit();

    // Load calibration files
    for (std::string& f : calibFiles)
      if (canLoad(currentDir + "/" + f)) load(currentDir + "/" + f);
  }
}

void CalibLoader::handleEvent(sofa::core::objectmodel::Event* e)
{
  if (sofa::simulation::AnimateBeginEvent::checkEventType(e)) this->update();
}

}  // namespace common
}  // namespace OR
}  // namespace sofa
