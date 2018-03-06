#include "MathTransform.h"

V_RECT RotateRectangle(RECT r, CVector2 a, float rot)
{
	V_RECT ret;

	ret.TL = RotatePoint(CVector2(r.left, r.top), a, rot);
	ret.TR = RotatePoint(CVector2(r.right, r.top), a, rot);
	ret.BL = RotatePoint(CVector2(r.left, r.bottom), a, rot);
	ret.BR = RotatePoint(CVector2(r.right, r.bottom), a, rot);

	return ret;
}

CVector2 RotatePoint(CVector2 p, CVector2 a, float rot)
{
	CVector2 ret;
	float s = sinf(rot);
	float c = cosf(rot);

	ret.x = p.x - a.x;
	ret.y = p.y - a.y;

	float xnew = ret.x * c - ret.y * s;
	float ynew = ret.x * s + ret.y * c;

	ret.x = xnew + a.x;
	ret.y = ynew + a.y;

	return ret;
}

bool GetIntersect(CVector2& i, CLine a, CLine b)
{
	if (a.Horizontal())
	{
		if (b.Horizontal())
		{
			if (a.GetY(1.0f) == b.GetY(1.0f))
			{
				i.y = a.GetY(1.0f);
				i.x = -1.0f;
			}
			else
				return false;
		}
		else if (b.Vertical())
		{
			i.x = b.GetX(1.0f);
			i.y = a.GetY(1.0f);
		}
		else
		{
			i.y = a.GetY(1.0f);
			i.x = b.GetY(i.y);
		}
	}
	else if (a.Vertical())
	{
		if (b.Vertical())
		{
			if (a.GetX(1.0f) == b.GetX(1.0f))
			{
				i.x = a.GetX(1.0f);
				i.y = -1.0f;
			}
			else
				return false;
		}
		else if (b.Horizontal())
		{
			i.x = a.GetX(1.0f);
			i.y = b.GetY(1.0f);
		}
		else
		{
			i.x = a.GetX(1.0f);
			i.y = b.GetY(i.x);
		}
	}
	else
	{
		if (b.Vertical())
		{
			i.x = b.GetX(1.0f);
			i.y = a.GetY(i.x);
		}
		else if (b.Horizontal())
		{
			i.y = b.GetY(1.0f);
			i.x = a.GetX(i.y);
		}
		else
		{
			i.x = (b.GetC() - a.GetC()) / (a.GetM() - b.GetM());
			i.y = a.GetY(i.x);
		}
	}
	return true;
}

CVector3 Lerp(CVector3 to, CVector3 from, float factor)
{
	return from + CVector3(to-from)*factor;
}

CVector2 Lerp(CVector2 to, CVector2 from, float factor)
{
	CVector3 v = Lerp(CVector3(to.x, to.y, 0.0f), CVector3(from.x, from.y, 0.0f), factor);
	return CVector2(v.x, v.y);
}
