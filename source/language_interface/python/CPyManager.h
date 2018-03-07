#pragma once

#include <string>
#include <vector>

#include "CPyFile.h"

class CPyManager
{
public:
	CPyManager(std::string prog_name);
	~CPyManager();
	int PYInit();
	int PYFinalize();

	int 		numScripts();
	std::string 	getName(int i);

	CPyObject callModuleFunc(std::string file, std::string func, std::vector<PY_ARG> args);

	bool AddScript(std::string file);
	bool RunScript(std::string file);
	bool RemScript(std::string file);
private:
	CPyFile* get_file(std::string file);

	std::wstring 		m_program;
	std::string 		m_program_str;
	std::vector<CPyFile> 	m_pyFile;
};