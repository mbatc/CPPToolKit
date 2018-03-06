#include "PyTypes.h"

#include "CPyObject.h"

bool pyarg_verify_match(std::vector<PY_ARG> c_arg, PyObject * py_object)
{
	if (PyTuple_Size(py_object) != c_arg.size())
		return false;
	for (int i = 0; i < PyTuple_Size(py_object); i++)
	{
		if(i >= c_arg.size())
		CPyObject item = PyTuple_GetItem(py_object, i);
		if (get_pyType(item) != c_arg[i].type_id)
			return false;
	}
	return true;
}
