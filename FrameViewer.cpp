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
    : d_frame(initData(&d_frame, "img", "frame to display in opencv window"))
{
  f_listening.setValue(true);
  static int i = 0;
  m_winID = std::string("FrameViewer");
  m_winID += std::to_string(i);
	cv::namedWindow(m_winID,
									CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
  i++;
}

FrameViewer::~FrameViewer() {}
void FrameViewer::init() { addInput(&d_frame); }
void FrameViewer::update()
{
  cv::Mat img = d_frame.getValue();
  std::cout << "Viewing" << std::endl;
  if (!d_frame.getValue().empty())
  {
    if (d_frame.getValue().type() == CV_32FC1)
    {
      msg_warning("FrameViewer::update()")
          << "CV_32F matrices will be normalized into a CV_8U matrix. Consider "
             "converting first to optimize performances";
      cv::normalize(d_frame.getValue(), img, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    }
    cv::imshow(m_winID, img);
		std::cout << "frameviewer" << std::endl;
    cv::waitKey(1);
  }
}

}  // namespace common
}  // namespace OR
}  // namespace sofa
