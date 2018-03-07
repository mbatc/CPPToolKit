#pragma once

#include "CPyObject.h"
#include "../../tools/CLog.h"

class CPyInstance
{
public:
	CPyInstance() {
		Log(this, DEBUGLOG_LEVEL_INFO, "[PYTHON] Python interpreter initialised.");
		Py_Initialize();
	}
	~CPyInstance() {
		Log(this, DEBUGLOG_LEVEL_INFO, "[PYTHON] Python interpreter destroyed.");
		Py_Finalize();
	}
};