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
		_LOG.Log(this, DEBUGLOG_LEVEL_ERROR, "Unable to open python file: %s",
			m_py_file_name.c_str());
		m_bValid = false;
		return m_bValid;
	}

	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "Successfully opened python file: %s",
		m_py_file_name.c_str());

	//Import module so that individual functions can be called



	return m_bValid;
}

int CPyFile::Run(int argc, char** argv)
{
	if (!m_pCpp_file)
	{
		_LOG.Log(this, DEBUGLOG_LEVEL_WARNING, "Python file (%s) has not been initialized",
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
		_LOG.Log(this, DEBUGLOG_LEVEL_WARNING, "Python file failed to run (%s): Unhandled exception (args[%s])",
			m_py_file_name.c_str(), args.c_str());
		return 1;
	}
	return 0;
}

int CPyFile::Call(std::string func, std::vector<PY_ARG> args)
{
	return 0;
}

bool CPyFile::valid()
{
	return m_bValid;
}