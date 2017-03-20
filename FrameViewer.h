#ifndef SOFA_OR_ACQUISITOR_FRAMEVIEWER_H
#define SOFA_OR_ACQUISITOR_FRAMEVIEWER_H

#include "SofaORCommon/cvMat.h"
#include "initplugin.h"

#include "SofaORCommon/ImplicitDataEngine.h"

#include <sofa/helper/OptionsGroup.h>
#include <SofaBaseVisual/VisualModelImpl.h>
#include <sofa/helper/Quater.h>

#include <opencv2/opencv.hpp>

namespace sofa
{
namespace OR
{
namespace common
{
class FrameViewer : virtual public ImplicitDataEngine
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

	Data<common::cvMat> d_frame;
	Data<defaulttype::Mat3x4d> d_P;
	Data<float> d_depth;

//	Data<defaulttype::Vec3f> d_translation;
//	Data<defaulttype::Vec3f> d_rotation;
//	Data<float> d_scale;
	Data<helper::OptionsGroup> d_mode;
	std::string m_winID;

private:
	float x0, y0;
	helper::vector<defaulttype::Vec3f> m_vecCoord;

	void perspectiveDraw();
	void orthoDraw();

//	void applyTranslation(const float dx, const float dy, const float dz);

//	void applyRotation (const float rx, const float ry, const float rz);

//	void applyRotation(const sofa::defaulttype::Quat q);

//	void applyScale(const float sx, const float sy, const float sz);


};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_ACQUISITOR_FRAMEVIEWER_H
