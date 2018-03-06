#pragma once

#include <Python.h>
#include <vector>

#define PY_TYPE_LONG	0x01
#define PY_TYPE_INT		0x02
#define PY_TYPE_LIST	0x03
#define PY_TYPE_STRING	0x04
#define PY_TYPE_BYTES	0x05
#define PY_TYPE_FLOAT	0x06
#define PY_TYPE_DICT	0x07

#define TYPECHECK(pyTypeCheck,typeID,object) if(pyTypeCheck(object)) return typeID

struct PY_ARG
{
	unsigned int	type_id;
	void*			pVar;
};

unsigned int get_pyType(PyObject* o)
{
	TYPECHECK(PyFloat_Check, PY_TYPE_FLOAT, o);
	TYPECHECK(PyList_Check, PY_TYPE_LIST, o);
	TYPECHECK(PyNumber_Check, PY_TYPE_INT, o);
	TYPECHECK(PyDict_Check, PY_TYPE_DICT, o);
	TYPECHECK(PyBytes_Check, PY_TYPE_STRING, o);
}

bool pyarg_verify_match(std::vector<PY_ARG>, PyObject* py_object);

template <class T> PyObject* PyType_convert(unsigned int type_id, T c_value)
{
	switch (type_id)
	{

	};
}