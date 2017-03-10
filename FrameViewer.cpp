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
			d_scale(initData(&d_scale, 1.0f, "scale", "image scale")),
			d_mode(
					initData(&d_mode, "mode", "viewer mode (PERSPECTIVE, ORTHO, HIDDEN)"))
{
	sofa::helper::OptionsGroup *t = d_mode.beginEdit();
	t->setNames(3, "PERSPECTIVE", "ORTHO", "HIDDEN");
	t->setSelectedItem(0);
	d_mode.endEdit();

	m_x0 = m_y0 = 500.0f;
	//	static int i = 0;
	//	m_winID = std::string("FrameViewer");
	//	m_winID += std::to_string(i);
	//	i++;
}

FrameViewer::~FrameViewer() {}
void FrameViewer::init() { addInput(&d_frame); }
void FrameViewer::update()
{
	//	cv::Mat img = d_frame.getValue();
	//	std::cout << "Viewing" << std::endl;
	//	if (!d_frame.getValue().empty())
	//	{
	//		if (d_frame.getValue().type() == CV_32FC1)
	//		{
	//			msg_warning("FrameViewer::update()")
	//					<< "CV_32F matrices will be normalized
	//into
	// a
	// CV_8U
	// matrix. Consider "
	//						 "converting first to optimize
	// performances";
	//			cv::normalize(d_frame.getValue(), img, 0, 255,
	// cv::NORM_MINMAX,
	// CV_8UC1);
	//		}
	//		cv::imshow(m_winID, img);
	//		std::cout << "frameviewer" << std::endl;
	//		cv::waitKey(1);
	//	}
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

	m_x0 = (float)d_frame.getValue().cols * d_scale.getValue() / 2;
	m_y0 = (float)d_frame.getValue().rows * d_scale.getValue() / 2;
	glTexCoord2f(0, 1);

	glTexCoord2f(0, 1);
	glVertex3f(-m_x0, m_y0, 0.002);
	glTexCoord2f(1, 1);
	glVertex3f(m_x0, m_y0, 0.002);
	glTexCoord2f(1, 0);
	glVertex3f(m_x0, -m_y0, 0.002);
	glTexCoord2f(0, 0);
	glVertex3f(-m_x0, -m_y0, 0.002);
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

	glBegin(
			GL_QUADS);  // we draw a quad on the entire screen (0,0 1,0 1,1 0,1)
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
	double maxBBox[3] = {m_x0, m_y0, m_x0};
	double minBBox[3] = {-m_x0, -m_y0, -m_x0};
	this->f_bbox.setValue(
			params, sofa::defaulttype::TBoundingBox<double>(minBBox, maxBBox));
}

}  // namespace common
}  // namespace OR
}  // namespace sofa
