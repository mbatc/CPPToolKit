#pragma once

class CVector3
{
public:
	CVector3();
	CVector3(float x, float y, float z);
	~CVector3();
	
	float x,y,z;

	float Magnitude();

	CVector3 operator +  (CVector3 in)
	{
		CVector3 out;
		out.x = this->x + in.x;
		out.y = this->y + in.y;
		out.z = this->z + in.z;

		return out;
	}

	CVector3 operator - (CVector3 in)
	{
		CVector3 out;
		out.x = this->x - in.x;
		out.y = this->y - in.y;
		out.z = this->z - in.z;

		return out;
	}

	CVector3 operator += (CVector3 in)
	{
		this->x += in.x;
		this->y += in.y;
		this->z += in.z;

		return *this;
	}

	CVector3 operator * (float in)
	{
		CVector3 out;
		out.x = this->x * in;
		out.y = this->y * in;
		out.z = this->z * in;

		return out;
	}

	CVector3 operator * (CVector3 in)
	{
		CVector3 out;
		out.x = this->x * in.x;
		out.y = this->y * in.y;
		out.z = this->z * in.z;

		return out;
	}

	CVector3 operator *= (CVector3 in)
	{
		this->x *= in.x;
		this->y *= in.y;
		this->z *= in.z;

		return *this;
	}
};