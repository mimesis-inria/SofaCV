#ifndef SOFA_OR_ACQUISITOR_PCVIEWER_H
#define SOFA_OR_ACQUISITOR_PCVIEWER_H

#include "initplugin.h"

#include "SofaORCommon/ImplicitDataEngine.h"

namespace sofa
{
namespace OR
{
namespace common
{
class PCViewer : public ImplicitDataEngine
{
	typedef defaulttype::Vec3f Vec3f;
  typedef defaulttype::Vec<3, uint8_t> Vec3b;

 public:
  SOFA_CLASS(PCViewer, ImplicitDataEngine);

 public:
  PCViewer();
  virtual ~PCViewer();

  void init();
  void update();
  void computeBBox(const core::ExecParams*, bool);
  void draw(const core::visual::VisualParams*);
  void reinit();

	Data<float> d_size;
	Data<defaulttype::Matrix3> d_R;
	Data<defaulttype::Vector3> d_T;
	// INPUTS
	Data<helper::vector<Vec3f> > d_positions;
  Data<helper::vector<Vec3b> > d_colors;

private:
  std::vector<defaulttype::Vec4f> m_colors;
	std::vector<Vec3f> m_positions;
};

}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_ACQUISITOR_PCVIEWER_H
