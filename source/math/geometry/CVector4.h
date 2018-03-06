#pragma once

class CVector4
{
public:
	CVector4();
	CVector4(float x, float y, float z, float w);
	~CVector4();
	
	float x,y,z,w;

	float Magnitude();
};