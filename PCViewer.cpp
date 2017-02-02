#include "PCViewer.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/core/visual/DrawToolGL.h>

namespace sofa
{
namespace OR
{
namespace common
{
SOFA_DECL_CLASS(PCViewer)

int PCViewerClass =
    core::RegisterObject("component to visualize pointclouds in Sofa")
        .add<PCViewer>();

PCViewer::PCViewer()
    : d_size(initData(&d_size, 1.0f, "size", "point size")),
      d_positions(
          initData(&d_positions, "positions", "point cloud's positions")),
      d_colors(initData(&d_colors, "colors", "point cloud's point colors"))
{
  f_listening.setValue(true);
}

PCViewer::~PCViewer() {}
void PCViewer::init()
{
  bindInputData(&d_positions);
  bindInputData(&d_colors);
  setDirtyValue();
}

void PCViewer::update()
{
  if (!d_positions.isDirty()) return;
  updateAllInputsIfDirty();
  cleanDirty();
  const helper::vector<Vec3b>& c = d_colors.getValue();
  m_colors.resize(c.size());
  for (size_t i = 0; i < c.size(); ++i)
    m_colors[i].set(c[i][0] / 255.0f, c[i][1] / 255.0f, c[i][2] / 255.0f, 1.0f);
}

void PCViewer::draw(const core::visual::VisualParams*)
{
  core::visual::DrawToolGL dt;
  if (d_positions.getValue().size() > m_colors.size())
    dt.drawPoints(d_positions.getValue(), d_size.getValue(),
                  defaulttype::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
  else
    dt.drawPoints(d_positions.getValue(), d_size.getValue(), m_colors);
  dt.drawSpheres(d_positions.getValue(), d_size.getValue(),
                 defaulttype::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
}

void PCViewer::reinit() { update(); }
}  // namespace common
}  // namespace OR
}  // namespace sofa
