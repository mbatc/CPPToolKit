#include "CVector2.h"

#include "CVector2.h"

#include <cmath>

CVector2::CVector2(float x, float y)
	:
	x(x),
	y(y)
{}

CVector2::CVector2()
	:
	x(0.0f),
	y(0.0f)
{}

CVector2::~CVector2()
{}

float CVector2::Magnitude()
{
	float mag;
	
	mag = sqrt(x*x + y*y);

	return mag;
}