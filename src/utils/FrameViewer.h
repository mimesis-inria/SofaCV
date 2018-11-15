#ifndef SOFACV_COMMON_FRAMEVIEWER_H
#define SOFACV_COMMON_FRAMEVIEWER_H

#include "SofaCVPlugin.h"
#include "ImplicitDataEngine.h"
#include "datatypes/cvMat.h"

#include <SofaBaseVisual/VisualModelImpl.h>
#include <sofa/helper/OptionsGroup.h>
#include <sofa/helper/Quater.h>

namespace sofacv
{
namespace utils
{
class SOFA_SOFACV_API FrameViewer : virtual public ImplicitDataEngine
{
 public:
    SOFA_CLASS(FrameViewer, ImplicitDataEngine);

 public:
  FrameViewer();
  virtual ~FrameViewer();

  void init() override;
  void reinit() override;
  void doUpdate() override;
  void draw(const sofa::core::visual::VisualParams*) override;
  void computeBBox(const sofa::core::ExecParams* params, bool) override;

    sofa::Data<cvMat> d_frame;
	sofa::Data<sofa::helper::vector<sofa::defaulttype::Vector3> > d_corners;
	sofa::Data<sofa::helper::OptionsGroup> d_mode;
	sofa::Data<float> d_alpha;

 private:
	void perspectiveDraw();
	void orthoDraw();
	void bindGlTexture(const std::string& imageString);
    void drawImpl();
};

}  // namespace utils
}  // namespace sofaor
#endif  // SOFACV_COMMON_FRAMEVIEWER_H
