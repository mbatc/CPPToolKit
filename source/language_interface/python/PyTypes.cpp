#include "PyTypes.h"

#include "CPyObject.h"
#include "CPyInstance.h"

unsigned int get_pyType(PyObject * o)
{
	TYPECHECK(PyFloat_Check, PY_TYPE_FLOAT, o);
	TYPECHECK(PyList_Check, PY_TYPE_LIST, o);
	TYPECHECK(PyLong_Check, PY_TYPE_INT, o);
	TYPECHECK(PyDict_Check, PY_TYPE_DICT, o);
	TYPECHECK(PyBytes_Check, PY_TYPE_STRING, o);
	TYPECHECK(PyLong_Check, PY_TYPE_LONG, o);
	TYPECHECK(PyBool_Check, PY_TYPE_BOOL, o);
}

bool pyarg_verify_match(std::vector<PY_ARG> c_arg, PyObject * py_object, PyObject** out)
{
	if (PyTuple_Size(py_object) != c_arg.size())
		return false;

	if (!out)
		return true;

	//Convert to Python Object
	
	*out = PyTuple_New(c_arg.size());
	
	for (size_t i = 0; i < c_arg.size(); i++)
	{
		PyTuple_SetItem(*out, i, PyType_convert(c_arg[i].type_id, c_arg[i].pVar, c_arg[i].list_def));
	}

	return true;
}

PyObject * PyType_convert(unsigned int type_id, void * c_value, LIST_DEF list_def)
{
	PyObject* pyVal = nullptr;
	switch (type_id)
	{
	case PY_TYPE_FLOAT:
		pyVal = PyFloat_FromDouble(*(float*)c_value);
		break;
	case PY_TYPE_INT:
		pyVal = PyLong_FromLong(*(int*)c_value);
		break;
	case PY_TYPE_STRING:
		pyVal = PyBytes_FromString((*(std::string*)c_value).c_str());
		break;
	case PY_TYPE_LONG:
		pyVal = PyLong_FromLong(*(long*)c_value);
		break;
	case PY_TYPE_BOOL:
		pyVal = PyBool_FromLong(*(bool*)c_value);
		break;
	case PY_TYPE_LIST:
		pyVal = PyType_convert_list(list_def.type_id, c_value, list_def.size, list_def.item_size);
		break;
	};

	return pyVal;
}

PyObject * PyType_convert_list(unsigned int type_id, void * c_value, size_t list_size, size_t item_size)
{
	PyObject* pyVal = PyList_New(list_size);

	if (!pyVal)
		return nullptr;

	for (size_t i = 0; i < list_size; i++)
		PyList_SetItem(pyVal, i, PyType_convert(type_id, (void*)(static_cast<char*>(c_value) + (item_size*i))));
	return pyVal;
}

//CPyInstance must exist before calling this function
PyObject * PyArgs_get_func_arglist(PyObject * pModule, std::string funcName, PyObject** pFuncOut)
{
	CPyObject pDict = PyModule_GetDict(pModule);
	CPyObject pFunc = PyDict_GetItemString(pDict, funcName.c_str());
	if(pFuncOut)
		*pFuncOut = pFunc;

	CPyObject pInspectName = PyBytes_FromString("inspect");
	CPyObject pArgSpecName = PyBytes_FromString("getargspec");
	
	if (!pInspectName || !pArgSpecName)
		return nullptr;

	CPyObject pInspect = PyImport_Import(pInspectName);
	
	if (!pInspect)
		return nullptr;

	CPyObject argspec_tuple = PyObject_CallMethodObjArgs(pInspect, pArgSpecName, pFunc);

	if (!argspec_tuple)
		return nullptr;

	return PyTuple_GetItem(argspec_tuple, 0);
}
