#pragma once

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>

#define _DEBUG
#else
#include <Python.h>
#endif

#pragma comment(lib, "python36.lib")