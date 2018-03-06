#pragma once

class CVector2
{
public:
	CVector2();
	CVector2(float x, float y);
	~CVector2();
	
	float x,y;

	float Magnitude();

	CVector2 operator +  (CVector2 in)
	{
		CVector2 out;
		out.x = this->x + in.x;
		out.y = this->y + in.y;

		return out;
	}

	CVector2 operator - (CVector2 in)
	{
		CVector2 out;
		out.x = this->x - in.x;
		out.y = this->y - in.y;

		return out;
	}

	CVector2 operator += (CVector2 in)
	{
		this->x += in.x;
		this->y += in.y;

		return *this;
	}

	CVector2 operator * (float in)
	{
		CVector2 out;
		out.x = this->x * in;
		out.y = this->y * in;

		return out;
	}
};