#pragma once

#include "CVector2.h"

class CLine
{
public:
	CLine(CVector2 a, CVector2 b);

	void SetA(CVector2 a);
	void SetB(CVector2 b);

	float GetM();
	float GetC();

	float GetY(float x);
	float GetX(float y);

	bool Vertical() { return m_bVertical; }
	bool Horizontal() { return m_bHorizontal; }

	bool WithinGivenPoints(CVector2 p);
private:
	void CalcLine();

	float m_m;
	float m_c;

	bool m_bVertical;
	bool m_bHorizontal;

	CVector2 m_a;
	CVector2 m_b;
};