#pragma once

#include "CLine.h"
#include "Vectors.h"
#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

struct V_RECT
{
	CVector2 TL;
	CVector2 BL;
	CVector2 TR;
	CVector2 BR;
};

V_RECT RotateRectangle(RECT r, CVector2 a, float rot);

CVector2 RotatePoint(CVector2 p, CVector2 a, float rot);
CVector3 RotatePoint(CVector3 p, CVector3 a, CVector3 rot);

bool GetIntersect(CVector2& i, CLine a, CLine b);

CVector3 Lerp(CVector3 to, CVector3 from, float factor);
CVector2 Lerp(CVector2 to, CVector2 from, float factor);
