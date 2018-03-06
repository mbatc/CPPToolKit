#include "COutput.h"
#include "../tools/CLog.h"
COutput::COutput()
{
	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "Output class initialised");
}

COutput::~COutput()
{}

void COutput::OpenFile(string s, const char* t)
{
	for (int i = 0; i < m_pFile.size(); i++)
	{
		if (!strcmp(m_pFile[i]._name.c_str(), s.c_str()))
		{
			_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "File is already open: %s", s.c_str());
			return;
		}
	}

	SFILE file(s, t);
	m_pFile.push_back(file);
}

void COutput::CloseFile(string s)
{
	for (int i = 0; i < m_pFile.size(); i++)
	{
		if (!strcmp(m_pFile[i]._name.c_str(), s.c_str()))
		{
			m_pFile[i].close();
			m_pFile.erase(m_pFile.cbegin() + i);
			i--;
		}
	}
}

void COutput::CloseAllFiles()
{
	for (int i = 0; i < m_pFile.size(); i++)
	{
		m_pFile[i].close();
	}
	m_pFile.clear();
}

void COutput::Print(string format, va_list args)
{
	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "Output to console: %s", format.c_str());
	vprintf(format.c_str(), args);
}

void COutput::Print(string msg)
{
	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "Output to console: %s", msg.c_str());
	printf(msg.c_str());
}

void COutput::PrintF(string file, string format, va_list args)
{
	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "Output to file (%s): %s", file.c_str(), format.c_str());
	FILE* f = GetHandle(file);
	if (!f)
	{
		_LOG.Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to output to file (%s), file has not been opened", file.c_str());
		return;
	}

	vfprintf(f, format.c_str(), args);
}

void COutput::PrintF(string file, string msg)
{
	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "Output to file (%s): %s", file.c_str(), msg.c_str());
	FILE* f = GetHandle(file);

	if (!f)
	{
		_LOG.Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to output to file (%s), file has not been opened", file.c_str());
		return;
	}

	fprintf(f, msg.c_str());
}

void COutput::WriteF(string file, const void * data, size_t size, int element_count)
{
	fwrite(data, size, element_count, GetHandle(file));
}

void COutput::ReadF(string file, void * buffer, size_t size, int element_count)
{
	FILE* f = GetHandle(file);
	if (!f)
	{
		_LOG.Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to read from file file (%s), file has not been opened", file.c_str());
		return;
	}

	fread(buffer, size, element_count, f);
}

int COutput::ScanF(string file, string format, va_list args)
{
	FILE* f = GetHandle(file);
	if (!f)
	{
		_LOG.Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to read from file file (%s), file has not been opened", file.c_str());
		return -1;
	}

	return vfscanf(f, format.c_str(), args);
}

FILE* COutput::GetHandle(string file)
{
	for (int i = 0; i < m_pFile.size(); i++)
	{
		if (!strcmp(m_pFile[i]._name.c_str(), file.c_str()))
			return m_pFile[i]._hFile;
	}

	return 0;
}

FILE* COutput::GetHandle(int index)
{
	if (index < 0 || index >= m_pFile.size())
		return 0;

	return m_pFile[index]._hFile;
}

COutput::SFILE::SFILE(string file, const char* t)
	:
	_hFile(0),
	_name("")
{
	if (file.c_str())
		_hFile = fopen(file.c_str(), t);

	if (!_hFile)
		_LOG.Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to open file '%s'", file.c_str());
	else
		_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "File Opened: %s", file.c_str());
	_name = file;
}

COutput::SFILE::~SFILE()
{}

void COutput::SFILE::close()
{
	if (_hFile)
	{
		fclose(_hFile);
		_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "File closed: %s", _name.c_str());
	}
}