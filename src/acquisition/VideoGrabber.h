/******************************************************************************
 *       SOFAOR, SOFA plugin for the Operating Room, development version       *
 *                        (c) 2017 INRIA, MIMESIS Team                         *
 *                                                                             *
 * This program is a free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation; either version 1.0 of the License, or (at     *
 * your option) any later version.                                             *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
 * for more details.                                                           *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.        *
 *******************************************************************************
 * Authors: Bruno Marques and external contributors (see Authors.txt)          *
 *                                                                             *
 * Contact information: contact-mimesis@inria.fr                               *
 ******************************************************************************/

#ifndef SOFACV_ACQUISITION_VIDEOGRABBER_H
#define SOFACV_ACQUISITION_VIDEOGRABBER_H

#include "ImplicitDataEngine.h"
#include "SofaCVPlugin.h"

#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/OptionsGroup.h>
#include "BaseFrameGrabber.h"

#include <opencv2/opencv.hpp>

#include <mutex>
#include <thread>

namespace sofacv
{
namespace acquisition
{
/**
 * \brief Video module
 */
namespace video
{
/**
 * \brief The VideoGrabber class can load video streams from the network or from
 * a connected webcam, but also images, video files, image sequences from the
 * filesystem
 *
 * It wraps OpenCV's VideoCapture component and allows navigation in the video
 * stream, if the stream permits it.
 * For more information, take a look at OpenCV's documentation on VideoCapture:
 * http://docs.opencv.org/3.2.0/d8/dfe/classcv_1_1VideoCapture.html
 */
class SOFA_SOFACV_API VideoGrabber : public BaseFrameGrabber
{
 public:
  SOFA_CLASS(VideoGrabber, BaseFrameGrabber);

 public:
  sofa::core::objectmodel::DataFileName
      d_fileName;  ///< [INTPUT] Name of the opened video
                   /// file, image sequence (eg.
  /// img_%02d.jpg), or network URL
  ///(PROTOCOL://SERVER_IP:PORT/PATH/TO/FILE)
  sofa::Data<int> d_seekFrame;  ///< Sets the position of the video file in
                                /// miliseconds or video capture timestamp
  sofa::Data<int> d_camIdx;  ///< [INTPUT] Index of the capturing device to open
  sofa::Data<bool> d_paused;   ///< [INTPUT] Toggle play / pause
  sofa::Data<bool> d_stopped;  ///< [INTPUT] Toggle play / stop
  sofa::Data<sofa::defaulttype::Vec2i>
      d_resolution;  ///< [INTPUT] To set specific camera grabbing resolution
  sofa::Data<bool> d_async;  ///< [INTPUT] separate thread for video read

  cv::VideoCapture m_cap;

 public:
  VideoGrabber();
  virtual ~VideoGrabber() override;

  void init() override;
  void doUpdate() override;
  void reinit() override;
  void cleanup() override;

 protected:
  void grabFrame();
  void grabFrames();
  void retrieveFrame();

 private:
  std::string m_filename;
  int m_camIdx;
  bool m_paused;
  bool m_stopped;
  bool m_async;
  int m_seek;
  bool m_isRunning;
  cvMat m_left, m_right, m_frame;
  std::thread t;
  std::mutex m;

 public:
  void StoppedChanged();
  void PausedChanged();
  void SeekChanged();
  void FileNameChanged();
  void CamIdxChanged();
  void AsyncChanged();
};

}  // namespace video
}  // namespace acquisition
}  // namespace sofacv

#endif  // SOFACV_ACQUISITION_VIDEOGRABBER_H
