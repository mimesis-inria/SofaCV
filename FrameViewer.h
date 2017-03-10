#ifndef SOFA_OR_ACQUISITOR_FRAMEVIEWER_H
#define SOFA_OR_ACQUISITOR_FRAMEVIEWER_H

#include "SofaORCommon/cvMat.h"
#include "initplugin.h"

#include "SofaORCommon/ImplicitDataEngine.h"

#include <sofa/helper/OptionsGroup.h>
#include <sofa/core/visual/VisualManager.h>

#include <opencv2/opencv.hpp>

namespace sofa
{
namespace OR
{
namespace common
{
class FrameViewer : public ImplicitDataEngine
{
 public:
	SOFA_CLASS(FrameViewer, ImplicitDataEngine);

 public:
  FrameViewer();
  virtual ~FrameViewer();

  void init();
  void update();
	void draw(const core::visual::VisualParams *);
	void computeBBox(const core::ExecParams* params, bool);

  sofa::Data<common::cvMat> d_frame;
	sofa::Data<float> d_scale;
	sofa::Data<helper::OptionsGroup> d_mode;
	std::string m_winID;

private:
	float m_x0, m_y0;

	void perspectiveDraw();
	void orthoDraw();
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_ACQUISITOR_FRAMEVIEWER_H
