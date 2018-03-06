#include "CVector3.h"

#include <cmath>

CVector3::CVector3(float x, float y, float z)
	:
	x(x),
	y(y),
	z(z)
{}

CVector3::CVector3()
	:
	x(0.0f),
	y(0.0f),
	z(0.0f)
{}

CVector3::~CVector3()
{}

float CVector3::Magnitude()
{
	float mag;
	
	mag = sqrt(x*x + y*y + z*z);

	return mag;
}