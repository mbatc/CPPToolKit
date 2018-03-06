#pragma once

#include "CPyObject.h"

#include "PyTypes.h"

#include <string>
#include <vector>

class CPyFile
{
public:
	CPyFile(std::string filename);
	~CPyFile();

	int Run(int argv = 0, char** argc = 0);
	int Call(std::string func, std::vector<PY_ARG> args);

	std::string getName();

	bool valid();
private:
	bool initialize();

	FILE*		m_pCpp_file;

	CPyObject 	m_pPy_file_obj;
	CPyObject	m_pyModule;

	std::string	m_py_file_name;
	bool 		m_bValid;
};