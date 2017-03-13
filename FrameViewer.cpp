#include "FrameViewer.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/AnimateBeginEvent.h>

#include <sofa/core/visual/DrawToolGL.h>
#include <sofa/helper/gl/RAII.h>
#include <sofa/helper/system/gl.h>
#include <sofa/helper/system/glut.h>

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
		: d_frame(initData(&d_frame, "img", "frame to display in opencv window")),
			d_translation(
					initData(&d_translation, "translation", "image translation")),
			d_rotation(initData(&d_rotation, "rotation", "image rotation")),
			d_scale(initData(&d_scale, 1.0f, "scale", "image scale")),
			d_mode(
					initData(&d_mode, "mode", "viewer mode (PERSPECTIVE, ORTHO, HIDDEN)"))
{
	sofa::helper::OptionsGroup *t = d_mode.beginEdit();
	t->setNames(3, "PERSPECTIVE", "ORTHO", "HIDDEN");
	t->setSelectedItem(0);
	d_mode.endEdit();

	x0 = y0 = 500.0f;
}

FrameViewer::~FrameViewer() {}
void FrameViewer::init()
{
	addInput(&d_frame);

	update();
}

//void FrameViewer::reinit()
//{
//	update();
//}

void FrameViewer::update()
{
	// retrieve new image, and, give the new dimensions, reapply scale
	float _x0 = (float)d_frame.getValue().cols / 2;
	float _y0 = (float)d_frame.getValue().rows / 2;

	x0 = _x0;
	y0 = _y0;
	m_vecCoord.clear();
	m_vecCoord.push_back(defaulttype::Vec3f(-x0, y0, .0f));
	m_vecCoord.push_back(defaulttype::Vec3f(x0, y0, .0f));
	m_vecCoord.push_back(defaulttype::Vec3f(x0, -y0, .0f));
	m_vecCoord.push_back(defaulttype::Vec3f(-x0, -y0, .0f));

	applyScale(d_scale.getValue(), d_scale.getValue(), d_scale.getValue());
	applyTranslation(d_translation.getValue()[0], d_translation.getValue()[1],
			d_translation.getValue()[2]);
	applyRotation(d_rotation.getValue()[0], d_rotation.getValue()[1],
			d_rotation.getValue()[2]);
}

void FrameViewer::perspectiveDraw()
{
	std::stringstream imageString;
	imageString.write((const char *)d_frame.getValue().data,
										d_frame.getValue().total() * d_frame.getValue().elemSize());

	glEnable(GL_TEXTURE_2D);  // enable the texture
	glDisable(GL_LIGHTING);   // disable the light

	glBindTexture(GL_TEXTURE_2D, 0);  // texture bind
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, d_frame.getValue().cols,
							 d_frame.getValue().rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE,
							 imageString.str().c_str());
	// glTexImage2D (GL_TEXTURE_2D, 0, GL_LUMINANCE, m_imageWidth,
	// m_imageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_imgData.c_str() );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
									GL_LINEAR);  // Linear Filtering

	//		float eps = 0.0;
	//		float z0 = 0.0;

	glBegin(GL_QUADS);  // we draw a quad on the entire screen (0,1 - 1,1 - 1,0
											// - 0,0)
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

	glTexCoord2f(0, 1);

	glTexCoord2f(0, 1);
	glVertex3f(m_vecCoord[0][0], m_vecCoord[0][1], m_vecCoord[0][2]);
	glTexCoord2f(1, 1);
	glVertex3f(m_vecCoord[1][0], m_vecCoord[1][1], m_vecCoord[1][2]);
	glTexCoord2f(1, 0);
	glVertex3f(m_vecCoord[2][0], m_vecCoord[2][1], m_vecCoord[2][2]);
	glTexCoord2f(0, 0);
	glVertex3f(m_vecCoord[3][0], m_vecCoord[3][1], m_vecCoord[3][2]);
	glEnd();

	// glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);     // enable light
	glDisable(GL_TEXTURE_2D);  // disable texture 2D
														 // glDepthMask (GL_TRUE);		// enable zBuffer
}

void FrameViewer::orthoDraw()
{
	std::stringstream imageString;
	imageString.write((const char *)d_frame.getValue().data,
										d_frame.getValue().total() * d_frame.getValue().elemSize());

	glMatrixMode(GL_PROJECTION);  // init the projection matrix
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);  // orthogonal view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// BACKGROUND TEXTURING
	// glDepthMask (GL_FALSE);		// disable the writing of
	// zBuffer
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);  // enable the texture
	glDisable(GL_LIGHTING);   // disable the light

	glBindTexture(GL_TEXTURE_2D, 0);  // texture bind
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, d_frame.getValue().cols,
							 d_frame.getValue().rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE,
							 imageString.str().c_str());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
									GL_LINEAR);  // Linear Filtering

	// BACKGROUND DRAWING
	// glEnable(GL_DEPTH_TEST);

	glBegin(GL_QUADS);  // we draw a quad on the entire screen (0,0 1,0 1,1 0,1)
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 1);
	glVertex2f(0, 0);
	glTexCoord2f(1, 1);
	glVertex2f(1, 0);
	glTexCoord2f(1, 0);
	glVertex2f(1, 1);
	glTexCoord2f(0, 0);
	glVertex2f(0, 1);
	glEnd();

	// glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);     // enable light
	glDisable(GL_TEXTURE_2D);  // disable texture 2D
	glEnable(GL_DEPTH_TEST);
	// glDepthMask (GL_TRUE);		// enable zBuffer

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void FrameViewer::draw(const core::visual::VisualParams *)
{
	GLfloat projectionMatrixData[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrixData);
	GLfloat modelviewMatrixData[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrixData);

	if (d_frame.getValue().data)
	{
		switch (d_mode.getValue().getSelectedId())
		{
			case 0:
				perspectiveDraw();
				break;
			case 1:
				orthoDraw();
				break;
			default:
				break;
		}
	}
}

void FrameViewer::computeBBox(const core::ExecParams *params, bool)
{
	const helper::vector<defaulttype::Vec3f> &x = m_vecCoord;

	double minBBox[3] = {std::numeric_limits<double>::max(),
											 std::numeric_limits<double>::max(),
											 std::numeric_limits<double>::max()};
	double maxBBox[3] = {-std::numeric_limits<double>::max(),
											 -std::numeric_limits<double>::max(),
											 -std::numeric_limits<double>::max()};
	for (unsigned int i = 0; i < x.size(); i++)
	{
		const defaulttype::Vec3f &p = x[i];
		for (int c = 0; c < 3; c++)
		{
			if (p[c] > maxBBox[c]) maxBBox[c] = p[c];
			if (p[c] < minBBox[c]) minBBox[c] = p[c];
		}
	}
	this->f_bbox.setValue(
			params, sofa::defaulttype::TBoundingBox<double>(minBBox, maxBBox));
}

void FrameViewer::applyTranslation(const float dx, const float dy,
																	 const float dz)
{
	defaulttype::Vec3f d(dx, dy, dz);

	for (unsigned int i = 0; i < m_vecCoord.size(); i++) m_vecCoord[i] += d;
}

void FrameViewer::applyRotation(const float rx, const float ry, const float rz)
{
	defaulttype::Quat q = helper::Quater<float>::createQuaterFromEuler(
			defaulttype::Vec3f(rx, ry, rz) * M_PI / 180.0);
	applyRotation(q);
}

void FrameViewer::applyRotation(const defaulttype::Quat q)
{
	for (unsigned int i = 0; i < m_vecCoord.size(); i++)
		m_vecCoord[i] = q.rotate(m_vecCoord[i]);
}

void FrameViewer::applyScale(const float sx, const float sy, const float sz)
{
	for (unsigned int i = 0; i < m_vecCoord.size(); i++)
	{
		m_vecCoord[i][0] *= sx;
		m_vecCoord[i][1] *= sy;
		m_vecCoord[i][2] *= sz;
	}
}

}  // namespace common
}  // namespace OR
}  // namespace sofa
