#include "FrameViewer.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/AnimateBeginEvent.h>

namespace sofa
{
namespace OR
{
namespace common
{
SOFA_DECL_CLASS(FrameViewer)

int FrameViewerClass =
    core::RegisterObject(
        "debug component to visualize images using OpenCV highgui")
        .add<FrameViewer>();

FrameViewer::FrameViewer()
    : d_frame(initData(&d_frame, "frame", "frame to display in opencv window"))
{
  f_listening.setValue(true);
  static int i = 0;
  m_winID = std::string("FrameViewer");
  m_winID += std::to_string(i);
  cv::namedWindow(m_winID, CV_WINDOW_AUTOSIZE);
  i++;
}

FrameViewer::~FrameViewer() {}
void FrameViewer::init()
{
  addInput(&d_frame);
  d_frame.setDirtyValue();
}

void FrameViewer::update()
{
  updateAllInputsIfDirty();
  cleanDirty();
  std::cout << "Viewing" << std::endl;
  if (!d_frame.getValue().empty())
  {
    cv::imshow(m_winID, d_frame.getValue());
    cv::waitKey(1);
  }
}

void FrameViewer::reinit() { update(); }
void FrameViewer::handleEvent(sofa::core::objectmodel::Event* e)
{
  if (sofa::simulation::AnimateBeginEvent::checkEventType(e)) this->update();
}

}  // namespace common
}  // namespace OR
}  // namespace sofa
