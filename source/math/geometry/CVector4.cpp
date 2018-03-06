#include "CVector4.h"

#include <cmath>

CVector4::CVector4(float x, float y, float z, float w)
	:
	x(x),
	y(y),
	z(z),
	w(w)
{}

CVector4::CVector4()
	:
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(0.0f)
{}

CVector4::~CVector4()
{}

float CVector4::Magnitude()
{
	float mag;
	
	mag = sqrt(x*x + y*y + z*z + w*w);

	return mag;
}