#include "CameraUtils.h"

namespace sofa
{
namespace OR
{
namespace common
{
namespace camera
{

// returns the 2D position in the image, from its projection matrix and 3D
// position in space
Vector2 get2DFrom3DPosition(const Vector3& p, const Mat3x4d& P)
{
	double rx = P[0][0] * p[0] + P[0][1] * p[1] + P[0][2] * p[2] + P[0][3];
	double ry = P[1][0] * p[0] + P[1][1] * p[1] + P[1][2] * p[2] + P[1][3];
	double rz = P[2][0] * p[0] + P[2][1] * p[1] + P[2][2] * p[2] + P[2][3];

	return Vector2(rx, ry) * 1.0 / rz;
}

// returns the 3D position of a 2D point 'x, y', projected using 'P' on a plane
// set at a
// camera distance 'd'
Vector3 get3DFrom2DPosition(double x, double y, const Mat3x4d& P, float d)
{
	Matrix3 C;
	Vector3 T;

	for (unsigned j = 0; j < 3; j++)
	{
		for (unsigned i = 0; i < 3; i++)
		{
			C[j][i] = P[j][i];
		}
		T[j] = P[j][3];
	}

	if (oneNorm(C) == 0) return Vector3();

	Matrix3 iC;
	iC.invert(C);

	Vector3 camera_pos = -iC * T;

	return iC * Vector3(x, y, 1) * d + camera_pos;
}

Vector3 get3DFrom2DPosition(const Vector2& p, const Mat3x4d& P, float d)
{
	return get3DFrom2DPosition(p[0], p[1], P, d);
}

// Returns the corners of an image of dimensions w:h, projected at distance 'd'
// from the camera, given its matrix P
void getCornersPosition(Vector3& p1, Vector3& p2, Vector3& p3, Vector3& p4,
												int w, int h, const Mat3x4d& P, float d)
{
	p1 = get3DFrom2DPosition(0, 0, P, d);
	p2 = get3DFrom2DPosition(0, h, P, d);
	p3 = get3DFrom2DPosition(w, h, P, d);
	p4 = get3DFrom2DPosition(w, 0, P, d);
}

}
}
}
}
