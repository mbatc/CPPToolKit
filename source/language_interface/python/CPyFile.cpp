#include "CPyFile.h"
#include "../../tools/CLog.h"

using namespace std;

CPyFile::CPyFile(std::string file)
	:
	m_py_file_name(file)
{
	m_bValid = initialize();
}

CPyFile::~CPyFile()
{}

string CPyFile::getName()
{
	return m_py_file_name;
}

bool CPyFile::initialize()
{
	//Open entire script
	m_bValid = true;
	m_pPy_file_obj = Py_BuildValue("s", m_py_file_name.c_str());
	m_pCpp_file = _Py_fopen_obj(m_pPy_file_obj, "r+");

	if (!m_pCpp_file)
	{
		_LOG.Log(this, DEBUGLOG_LEVEL_ERROR, "[PYTHON] Unable to open file: %s",
			m_py_file_name.c_str());
		m_bValid = false;
		return m_bValid;
	}

	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "[PYTHON] Successfully opened python file: %s",
		m_py_file_name.c_str());

	//Import module so that individual functions can be called

	m_pyModule = PyImport_Import(m_pPy_file_obj);

	return m_bValid;
}

int CPyFile::Run(int argc, char** argv)
{
	if (!m_pCpp_file)
	{
		_LOG.Log(this, DEBUGLOG_LEVEL_WARNING, "[PYTHON] file (%s) has not been initialized",
			m_py_file_name.c_str());

		return 1;
	}
	std::string args = " ";
	for (int i = 0; i < argc; i++)
	{
		args += ", ";
		args += argv[0];
	}
	args[0] = ' ';
	if (PyRun_SimpleFile(m_pCpp_file, m_py_file_name.c_str()) < 0)
	{
		_LOG.Log(this, DEBUGLOG_LEVEL_WARNING, "[PYTHON] file failed to run (%s): Unhandled exception (args[%s])",
			m_py_file_name.c_str(), args.c_str());
		return 1;
	}
	return 0;
}

CPyObject CPyFile::Call(std::string func, std::vector<PY_ARG> args)
{
	CPyObject pFunc = nullptr;
	CPyObject pConvertedArg = nullptr;
	CPyObject arg_list = PyArgs_get_func_arglist(m_pyModule, func, pFunc);
	
	if (!pFunc)
	{
		Log(this, DEBUGLOG_LEVEL_ERROR, "[PYTHON] Failed to call %s:%s, Could not find object in module", m_py_file_name.c_str(),
			func.c_str());
	}

	if (!pyarg_verify_match(args, arg_list, pConvertedArg))
	{
		Log(this, DEBUGLOG_LEVEL_ERROR, "[PYTHON] Failed to call %s:%s, C-Args do not match Py-Args", m_py_file_name.c_str(),
			func.c_str());
		return CPyObject();
	}

	if (!PyCallable_Check(pFunc))
	{
		Log(this, DEBUGLOG_LEVEL_ERROR, "[PYTHON] Failed to call %s:%s, Not a callable python object", m_py_file_name.c_str(),
			func.c_str());
		return CPyObject();
	}
	return PyObject_CallObject(pFunc, pConvertedArg);
}

bool CPyFile::valid()
{
	return m_bValid;
}