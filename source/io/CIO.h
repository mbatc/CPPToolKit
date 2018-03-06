#pragma once

#include "CInput.h"
#include "COutput.h"

class CIO
{
public:
	CIO();
	~CIO();

	void Print(std::string format, ...);
	void PrintF(std::string file, std::string format, ...);
	void WriteF(std::string file, const void* data, size_t data_size, size_t element_count);

	void ReadF(std::string file, void* buffer, size_t size, int element_count);
	void ScanF(std::string file, std::string format, ...);

	void OFStream(std::string file, std::string format, ...);
	std::string IFStream(std::string file);

	std::string GetCMDLine();

	void OpenFile(std::string file, const char* t);
	void CloseFile(std::string file);
	void CloseAllFiles();

	CInput _in;
	COutput _out;
};

extern CIO _io;