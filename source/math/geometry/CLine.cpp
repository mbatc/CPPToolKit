#include "CLine.h"

CLine::CLine(CVector2 a, CVector2 b)
{
	m_a = a;
	m_b = b;

	CalcLine();
}

void CLine::SetA(CVector2 a)
{
	m_a = a;
	CalcLine();
}

void CLine::SetB(CVector2 b)
{
	m_b = b;
	CalcLine();
}

float CLine::GetM()
{
	return m_m;
}

float CLine::GetC()
{
	return m_c;
}

float CLine::GetY(float x)
{
	float y = 0.0f;
	if (m_bHorizontal)
	{
		y = m_c;
	}
	else if (m_bVertical)
	{
		y = -0.1f;
	}
	else
	{
		y = m_m*x + m_c;
	}
	return y;
}

float CLine::GetX(float y)
{
	float x = 0;
	if (m_bHorizontal)
	{
		x = -1.0f;
	}
	else if (m_bVertical)
	{
		x = m_c;
	}
	else
	{
		x = (y - m_c) / m_m;
	}
	return x;
}

bool CLine::WithinGivenPoints(CVector2 p)
{
	if (m_bVertical)
	{
		if (m_a.y < m_b.y)
			return (p.y >= m_a.y && p.y <= m_b.y && p.x == GetX(1.0f));
		else
			return (p.y >= m_b.y && p.y <= m_a.y && p.x == GetX(1.0f));
	}
	else if (m_bHorizontal)
	{
		if (m_a.x < m_b.x)
			return (p.x >= m_a.x && p.x <= m_b.x && p.y == GetY(1.0f));
		else
			return (p.x >= m_b.x && p.x <= m_a.x && p.y == GetY(1.0f));
	}

	if (m_a.x < m_b.x)
		return (p.x >= m_a.x && p.x <= m_b.x && p.y == GetY(p.x));
	else
		return (p.x >= m_b.x && p.x <= m_a.x && p.y == GetY(p.x));

	return true;
}

void CLine::CalcLine()
{
	m_bHorizontal = false;
	m_bVertical = false;

	float rise = m_b.y - m_a.y;
	float run = m_b.x - m_a.x;

	if (rise == 0)
	{
		m_bHorizontal = true;
		m_c = m_a.y;
		m_m = 0;
	}
	else if (run == 0)
	{
		m_bVertical = true;
		m_c = m_a.x;
	}
	else
	{
		m_m = rise / run;
		m_c = m_a.y - m_m*m_a.x;
	}
}
