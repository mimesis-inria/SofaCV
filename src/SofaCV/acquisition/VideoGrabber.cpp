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

#include "VideoGrabber.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/AdvancedTimer.h>
#include <sofa/helper/system/FileRepository.h>
#include <sofa/helper/system/SetDirectory.h>
#include <sofa/simulation/AnimateBeginEvent.h>

namespace sofacv
{
namespace acquisition
{
namespace video
{
SOFA_DECL_CLASS(VideoGrabber)

static int VideoGrabberClass =
    sofa::core::RegisterObject(
        "OpenCV-based component reading mono and stereo videos")
        .add<VideoGrabber>();

VideoGrabber::VideoGrabber()
    : BaseFrameGrabber(),
      d_fileName(initData(
          &d_fileName, "videoFile",
          "you can provide either a video file (e.g. video.avi) or a sequence "
          "of images (img_%02d.jpg), or an URL for IP cams (e.g. "
          "protocol://host:port/script_name?script_params|auth). Note that "
          "each video stream or IP camera feed has its own URL scheme.")),
      d_seekFrame(initData(&d_seekFrame, 0, "seek",
                           "fast-forward video to reach a specific frame")),
      d_camIdx(initData(&d_camIdx, 0, "cam_index", "camera device index")),
      d_paused(initData(&d_paused, false, "paused", "if true, stops grabbing")),
      d_stopped(initData(&d_stopped, false, "stopped",
                         "if true, sets seek to 0 and paused to true")),
      d_resolution(initData(&d_resolution, sofa::defaulttype::Vec2i(1280, 720),
                            "resolution", "grabbing resolution to set")),
      d_async(initData(&d_async, false, "async",
                       "Asynchronous read of video frames")),
      m_filename(""),
      m_camIdx(0),
      m_seek(0)
{
  f_listening.setValue(true);
}

void VideoGrabber::init()
{
  // Differentiate between a file on the filesystem and an URL on the network
  // If m_filename is an image sequence, path must still be absolute, or
  // relative to current dir
  std::string s = d_fileName.getFullPath();
  std::ostringstream oss;
  if (sofa::helper::system::DataRepository.findFile(s, "", &oss)) //avoid printing msg_error...
    m_filename = d_fileName.getFullPath();
  else //findFile() wont be able to interpret "%06d"-style format
  {
      const int size = 1024;
      char buffer[size];
      // TODO: check if correct format string: https://www.owasp.org/index.php/Format_string_attack
      snprintf( buffer, size, d_fileName.getValue().c_str(), 1);
      std::string strFromFormat(buffer);
      if(sofa::helper::system::DataRepository.findFile(strFromFormat, "", &std::cerr))
      {
          std::string filenameWithFormat = sofa::helper::system::SetDirectory::GetFileName(d_fileName.getValue().c_str());
          std::string basedir = sofa::helper::system::SetDirectory::GetParentDir(strFromFormat.c_str());
          m_filename = basedir+"/"+ filenameWithFormat;
      }
      else
      {
          m_filename = d_fileName.getValue();
      }
  }

  m_stopped = d_stopped.getValue();
  m_paused = d_paused.getValue();

  if (m_filename.empty())
  {
    m_cap.open(d_camIdx.getValue());
    m_cap.set(cv::CAP_PROP_FRAME_WIDTH, d_resolution.getValue().x());
    m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, d_resolution.getValue().y());
  }
  else
  {
    m_frame = cv::imread(m_filename);
    if (m_frame.empty())
    {
      m_cap.open(m_filename);
      m_cap.set(cv::CAP_PROP_POS_FRAMES, d_seekFrame.getValue());
      m_seek = d_seekFrame.getValue();
    }
  }
  if (m_cap.isOpened() && m_cap.grab())
  {
    d_fps.setValue(m_cap.get(cv::CAP_PROP_FPS));
    sofa::defaulttype::Vec2i dims;
    dims[0] = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_WIDTH));
    dims[1] = static_cast<int>(m_cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    d_dimensions.setValue(dims);
    grabFrame();
    m_async = d_async.getValue();
    if (m_async && !m_isRunning)
    {
      t = std::thread(&VideoGrabber::grabFrames, this);
      m_isRunning = true;
    }
  }
  else if (m_frame.empty())
    msg_error(getName() + "::init()")
        << "VideoCapture::open(): Failed to open video stream " + m_filename;

  addInput(&d_stopped);
  addInput(&d_paused);
  addInput(&d_seekFrame);
  addInput(&d_fileName);
  addInput(&d_camIdx);
  addInput(&d_async);

  BaseFrameGrabber::init();
  update();
}

void VideoGrabber::grabFrame()
{
  if (m_stopped || m_paused || !m_cap.isOpened() || !f_listening.getValue())
    return;
  if (m_cap.retrieve(m_frame))
  {
    m.lock();
    if (m_filename != "")
      m_seek = int(m_cap.get(cv::CAP_PROP_POS_FRAMES));
    else
      m_seek++;
    splitFrames(
        m_frame, m_left, m_right,
        static_cast<image::VideoMode>(d_videoMode.getValue().getSelectedId()));
    m.unlock();
  }
  m_cap.grab();
}

void VideoGrabber::grabFrames()
{
  while (m_isRunning) grabFrame();
}

void VideoGrabber::retrieveFrame()
{
  m.lock();

  if (!m_frame.empty())
  {
    d_seekFrame.setValue(m_seek);
    m_dataTracker.clean(d_seekFrame);

    if (d_videoMode.getValue().getSelectedId() == image::MONO)
    {
      d_fullFrame.cleanDirty();
      cvMat& m = *d_fullFrame.beginEdit();
      m_frame.copyTo(m);
      d_fullFrame.endEdit();
      d_frame1.cleanDirty();
      m_frame.copyTo(*d_frame1.beginEdit());
      d_frame1.endEdit();
    }
    else
    {
      d_frame1.cleanDirty();
      m_frame.copyTo(*d_frame1.beginEdit());
      d_frame1.endEdit();
      d_frame2.cleanDirty();
      m_frame.copyTo(*d_frame2.beginEdit());
      d_frame2.endEdit();
      d_fullFrame.cleanDirty();
      m_frame.copyTo(*d_fullFrame.beginEdit());
      d_fullFrame.endEdit();
    }
    m_frame.release();
  }
  m.unlock();
}

void VideoGrabber::reinit()
{
  if (m_dataTracker.hasChanged(d_stopped))
  {
    StoppedChanged();
  }
  if (m_dataTracker.hasChanged(d_paused))
  {
    PausedChanged();
  }
  if (m_dataTracker.hasChanged(d_fileName))
  {
    FileNameChanged();
  }
  if (m_dataTracker.hasChanged(d_camIdx))
  {
    CamIdxChanged();
  }
  if (m_dataTracker.hasChanged(d_async))
  {
    AsyncChanged();
  }
  if (m_dataTracker.hasChanged(d_seekFrame))
  {
    SeekChanged();
  }
}


void VideoGrabber::doUpdate()
{
  sofa::helper::AdvancedTimer::stepBegin("RetrieveFrame");
  if (!m_async)
  {
    grabFrame();
  }
  retrieveFrame();
  sofa::helper::AdvancedTimer::stepEnd("RetrieveFrame");
}

void VideoGrabber::StoppedChanged()
{
  if (d_stopped.getValue())
  {
    if (m_async)
    {
      m_isRunning = false;
      t.join();
    }
    d_seekFrame.setValue(1);
    SeekChanged();
  }
  else
  {
    d_seekFrame.setValue(1);
    SeekChanged();
    if (m_async)
    {
      t = std::thread(&VideoGrabber::grabFrames, this);
      m_isRunning = true;
    }
  }
  // extra call to update to ensure frame is back to 1
  m_stopped = d_stopped.getValue();
}
void VideoGrabber::PausedChanged()
{
  if (d_paused.getValue())
  {
    if (m_async)
    {
      m_isRunning = false;
      t.join();
    }
  }
  else
  {
    if (m_async)
    {
      t = std::thread(&VideoGrabber::grabFrames, this);
      m_isRunning = true;
    }
  }
  m_paused = d_paused.getValue();
}

void VideoGrabber::SeekChanged()
{
  if (m_async)
  {
    m.lock();
  }
  m_seek = d_seekFrame.getValue();
  m_cap.set(cv::CAP_PROP_POS_FRAMES, m_seek);
  if (m_async)
  {
      m.unlock();
  }
  update();
}

void VideoGrabber::AsyncChanged()
{
  cleanup();
  init();
}

void VideoGrabber::FileNameChanged()
{
  d_seekFrame.setValue(1);
  cleanup();
  init();
}
void VideoGrabber::CamIdxChanged()
{
  d_seekFrame.setValue(1);
  cleanup();
  init();
}

void VideoGrabber::cleanup()
{
  if (m_async)
  {
    m_isRunning = false;
    t.join();
  }
  m_cap.release();
}

void VideoGrabber::grab(double elapsed)
{
    if (m_async)
    {
        msg_error(this->getName() + "::grab()") << "async mode is not supported when controlled by a Scheduler.";
        return;
    }
    if (d_interpolate.getValue())
    {
        // Do interpolation stuff
    }
    else
    {
        std::cout << elapsed << std::endl;
        while (elapsed > 0)
        {
            update();
            elapsed--;
        }

        /// Huh...: Seek is actually slower than grabbing X times the last frame.
//        m_seek += int(std::round(elapsed));
//        m_cap.set(CV_CAP_PROP_POS_FRAMES, m_seek);
//        d_seekFrame.setValue(m_seek);
//        m_dataTracker.clean(d_seekFrame);
//        update();
    }
}

VideoGrabber::~VideoGrabber() {}

}  // namespace video
}  // namespace acquisition
}  // namespace sofacv
