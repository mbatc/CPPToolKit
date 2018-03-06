#pragma once

#include "CPyObject.h"

class CPyInstance
{
public:
	CPyInstance() {
		Py_Initialize();
	}
	~CPyInstance() {
		Py_Finalize();
	}
};