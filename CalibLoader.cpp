#include "CalibLoader.h"
#include <cstring>
#include <fstream>
#include <string>

namespace sofa
{
namespace OR
{
namespace common
{
CalibLoader::CalibLoader()
    : d_calibFile(
          initData(&d_calibFile, "calib_file", "calibration file to load")),
      d_isStereo(
          initData(&d_isStereo, "is_stereo",
                   "set to true if the given calib file has stereo data")),
      d_leftCalib(initData(&d_leftCalib, "left_calib",
                           "camera Left (or default if mono)", true, true)),
      d_rightCalib(
          initData(&d_rightCalib, "right_calib", "camera right", true, true)),
      d_stereoCalib(initData(&d_stereoCalib, "stereo_calib",
                             "Rt, E and F matrices", true, true)),
      d_calibName(
          initData(&d_calibName, "calib_name", "Calibration name", true, true))
{
}

CalibLoader::~CalibLoader() {}
void CalibLoader::init()
{
  if (d_calibFile.getValue() == "")
  {
    msg_error("CalibLoader::canLoad()")
        << "Error: CalibLoader was given an empty calibName";
    return;
  }

  // -- Check if file exist:
  const char* filename = d_calibFile.getFullPath().c_str();
  std::string sfilename(filename);

  if (!sofa::helper::system::DataRepository.findFile(sfilename))
  {
    msg_error("CalibLoader::canLoad()") << "Error: File '" << d_calibFile
                                        << "' not found. ";
    return;
  }

  std::ifstream f(filename);

  // -- Check if file is readable:
  if (!f.good())
  {
    msg_error("CalibLoader::canLoad()") << "Error: Cannot read file '"
                                        << d_calibFile << "'.";
    return;
  }
  f.close();

  cv::FileStorage fs(filename, cv::FileStorage::READ);

  std::string stem = filename;

  size_t sep = stem.find_last_of("\\/");
  if (sep != std::string::npos)
    stem = stem.substr(sep + 1, stem.size() - sep - 1);

  size_t dot = stem.find_last_of(".");
  if (dot != std::string::npos) stem = stem.substr(0, dot);

  d_calibName.setValue(stem);

  cv::Mat_<double> cameraMatrixL;
  cv::Mat_<double> cameraMatrixR;
  cv::Mat_<double> distVecL;
  cv::Mat_<double> distVecR;
  cv::Mat_<double> R;
  cv::Mat_<double> T;
  cv::Mat_<double> F;
  double error;

  fs["cam_mats_left"] >> cameraMatrixL;
  fs["cam_mats_right"] >> cameraMatrixR;
  fs["dist_coefs_left"] >> distVecL;
  fs["dist_coefs_right"] >> distVecR;
  fs["rot_mat"] >> R;
  fs["trans_vec"] >> T;
  fs["f_mat"] >> F;
  fs["avgError"] >> error;

  d_leftCalib.setValue(common::CameraCalib(cameraMatrixL, distVecL, 0.0));
  d_rightCalib.setValue(common::CameraCalib(cameraMatrixR, distVecR, 0.0));
  d_stereoCalib.setValue(common::StereoCalib(R,T,F,error));

  fs.release();
}

void CalibLoader::update() {}
void CalibLoader::reinit() {}
}  // namespace common
}  // namespace OR
}  // namespace sofa
