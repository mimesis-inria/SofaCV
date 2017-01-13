#include "StereoViewer.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/AnimateBeginEvent.h>

namespace sofa
{
namespace OR
{
namespace common
{
SOFA_DECL_CLASS(StereoViewer)

int StereoViewerClass =
    core::RegisterObject(
        "debug component to visualize stereo frames using OpenCV's highgui")
        .add<StereoViewer>();

StereoViewer::StereoViewer()
    : d_frameL(initData(&d_frameL, "frameL",
                        "left frame to display in opencv window")),
      d_frameR(initData(&d_frameR, "frameR",
                        "right frame to display in opencv window"))
{
  f_listening.setValue(true);
  static int i = 0;
  m_winID = std::string("StereoViewer");
  m_winID += std::to_string(i);
  cv::namedWindow(m_winID, CV_WINDOW_AUTOSIZE);
  i++;
}

StereoViewer::~StereoViewer() {}
void StereoViewer::init()
{
  addInput(&d_frameL);
  addInput(&d_frameR);
  if (!d_frameL.getValue().empty() && !d_frameR.getValue().empty())
  {
    cv::hconcat(d_frameL.getValue(), d_frameR.getValue(), m_stereoFrame);
    cv::imshow(m_winID, m_stereoFrame);
    cv::waitKey(1);
  }
  setDirtyValue();
}

void StereoViewer::update()
{
  if (d_frameL.isDirty() && d_frameR.isDirty())
  {
    if (!d_frameL.getValue().empty() && !d_frameR.getValue().empty())
    {
      cv::hconcat(d_frameL.getValue(), d_frameR.getValue(), m_stereoFrame);
      cv::imshow(m_winID, m_stereoFrame);
      cv::waitKey(1);
    }
    d_frameL.cleanDirty();
    d_frameR.cleanDirty();
  }
}

void StereoViewer::reinit() { update(); }
void StereoViewer::handleEvent(sofa::core::objectmodel::Event* e)
{
  if (sofa::simulation::AnimateBeginEvent::checkEventType(e)) this->update();
}

}  // namespace common
}  // namespace OR
}  // namespace sofa
