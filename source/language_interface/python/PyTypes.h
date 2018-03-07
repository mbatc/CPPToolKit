#pragma once

#include "PyInclude.h"

#include <vector>

#define PY_TYPE_LONG	0x01
#define PY_TYPE_INT		0x02
#define PY_TYPE_LIST	0x03
#define PY_TYPE_STRING	0x04
#define PY_TYPE_FLOAT	0x06
#define PY_TYPE_DICT	0x07
#define PY_TYPE_BOOL	0x08

#define TYPECHECK(pyTypeCheck,typeID,object) if(pyTypeCheck(object)) return typeID

struct LIST_DEF
{
	LIST_DEF()
		:
		type_id(0),
		size(0),
		item_size(0) {}

	unsigned int	type_id;
	size_t			size;
	size_t			item_size;
};

struct PY_ARG
{
	unsigned int	type_id;
	void*			pVar;

	LIST_DEF		list_def;
};

unsigned int get_pyType(PyObject* o);

bool pyarg_verify_match(std::vector<PY_ARG>, PyObject* py_object, PyObject** out);

// PyType_convert(uint, T)
// Convert supported C-Types to python variables
// If PY_TYPE_STRING is passed c_value must be a std::string object
// type_id must reflect the c-type of the c_value being passed to the function
PyObject* PyType_convert(unsigned int type_id, void* c_value, LIST_DEF list_def = LIST_DEF());

//List can only contain objects that can be converted by the PyType_convert function
//	int
//	float
//	long
//	std::string
//	bool

PyObject* PyType_convert_list(unsigned int type_id, void* c_value, size_t list_size, size_t item_size);

PyObject* PyArgs_get_func_arglist(PyObject* pModule ,std::string func , PyObject** pFuncOut = nullptr);