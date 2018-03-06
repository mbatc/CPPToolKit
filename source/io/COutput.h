#pragma once
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

class COutput
{
public:
	COutput();
	~COutput();

	//Open a file for editing
	void OpenFile(string file, const char* t);
	//Close an open file
	void CloseFile(string file);
	//Close all open files
	void CloseAllFiles();

	//Print to console
	void Print(string format, va_list args);
	void Print(string  msg);
	//Print to file
	void PrintF(string file, string format, va_list args);
	void PrintF(string file, string msg);
	//Write data to file
	void WriteF(string file, const void* data, size_t size, int element_count);
	//Read data from file
	void ReadF(string file, void* buffer, size_t size, int element_count);
	int ScanF(string file, string format, va_list args);
private:
	FILE* GetHandle(string name);
	FILE* GetHandle(int i);

	struct SFILE
	{
		SFILE(string file, const char* t);
		~SFILE();

		void close();

		FILE* _hFile;
		string _name;
	};

	std::vector<SFILE> m_pFile;
};