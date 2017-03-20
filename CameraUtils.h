#ifndef SOFA_OR_COMMON_CAMERAUTILS_H
#define SOFA_OR_COMMON_CAMERAUTILS_H

#include <sofa/defaulttype/Mat.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/vector.h>

#include <igtlImageMessage.h>
#include <opencv2/opencv.hpp>

namespace sofa
{
namespace OR
{
namespace common
{
namespace camera
{
typedef defaulttype::Vector2 Vector2;
typedef defaulttype::Vector3 Vector3;
typedef defaulttype::Matrix3 Matrix3;
typedef defaulttype::Matrix4 Matrix4;
typedef defaulttype::Mat3x4d Mat3x4d;

// returns the 2D position in the image, from its projection matrix and 3D
// position in space
Vector2 get2DFrom3DPosition(const Vector3& p, const Mat3x4d& P);

// returns the 3D position of a 2D point 'x, y', projected using 'P' on a plane
// set at a
// camera distance 'd'
Vector3 get3DFrom2DPosition(double x, double y, const Mat3x4d& P, float d);

Vector3 get3DFrom2DPosition(const Vector2& p, const Mat3x4d& P, float d);

// Returns the corners of an image of dimensions w:h, projected at distance 'd'
// from the camera, given its matrix P
void getCornersPosition(Vector3& p1, Vector3& p2, Vector3& p3, Vector3& p4,
												int w, int h, const Mat3x4d& P, float d);

}  // namespace camera
}  // namespace common
}  // namespace OR
}  // namespace sofa

#endif  // SOFA_OR_COMMON_CAMERAUTILS_H
