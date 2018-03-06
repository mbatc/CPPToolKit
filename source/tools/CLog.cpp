#include "CLog.h"
#include <time.h>

CLog _LOG;

CLog::CLog(const char* file)
	:
	m_log_level(DEBUGLOG_LEVEL_ALL),
	m_bPrint_time(true),
	m_bCmd_log(false),
	m_sFile(file),
	m_sSignature("")
{
	memset(m_log_time, 0, sizeof(m_log_time));
}

CLog::~CLog()
{
	PrintFooter();
}

void CLog::PrintHeader()
{
	//Get formatted time in string
	//time_t _time;
	_strdate(m_log_time);/*
	sprintf(m_log_time, "%s", ctime(&_time));
	m_log_time[strlen(m_log_time)+1] = '\0';*/

	//Print time to header message
	char header_msg[256] = { 0 };
	sprintf(header_msg, "-- DEBUGLOG [%s] --", m_log_time);

	char header_top[256] = { '_' };
	memset(header_top, '_', sizeof(char) * 256);

	int l = strlen(header_msg);
	header_top[l + 1] = '\0';

	if (m_bClear)
		remove(m_sFile.c_str());

	//Print Formatted header
	PrintToFileF(false, header_top);
	PrintToFileF(false, header_msg);
}

void CLog::PrintFooter()
{
	char footer_msg[256] = { 0 };
	sprintf(footer_msg, "-- END OF DEBUG LOG [%s] --", m_log_time);

	char footer_b[256] = { '_' };
	memset(footer_b, '_', sizeof(char) * 256);

	int l = strlen(footer_msg);
	footer_b[l + 1] = '\0';

	//Print Formatted footer
	PrintToFileF(false, footer_msg);
	PrintToFileF(false, footer_b);
}

void CLog::SetLogLevel(unsigned int level)
{
	m_log_level = level;
	Log(this, DEBUGLOG_LEVEL_INFO, "Debug Log Level Set %s [%i]", GetLLString(level), level);
}

void CLog::SetLogFile(string file)
{
	m_sFile = file;
	PrintHeader();
}

void CLog::CMDLog(bool cmdLog)
{
	m_bCmd_log = cmdLog;
}

void CLog::Log(const void* src_obj, const unsigned int level, const char* format, ...)
{
	if (level >  m_log_level)
		return;

	//Generate and store the log signature
	GetSignature(src_obj, level);

	//Create the arguments object
	va_list args;

	//Print to the console if m_bCmd_log is TRUE
	if (m_bCmd_log)
	{
		//Get Argument list
		va_start(args, format);
		PrintToConsole(true, format, args);
		va_end(args);
	}

	//Get argument list
	va_start(args, format);
	PrintToFile(true, format, args);
	va_end(args);
}

void CLog::Log(const void* src_obj, const unsigned int level, const char* format, va_list args)
{
	if (level >  m_log_level)
		return;

	//Generate and store the log signature
	GetSignature(src_obj, level);

	//Print to the console if m_bCmd_log is TRUE
	if (m_bCmd_log)
	{
		//Get Argument list
		PrintToConsole(true, format, args);
	}

	//Get argument list
	PrintToFile(true, format, args);
}

void CLog::PrintToFileF(bool bSignature, const char* format, ...) const
{
	//Get argument list
	va_list args;
	va_start(args, format);

	//Print formatted string to file
	PrintToFile(bSignature, format, args);
	va_end(args);
}

void CLog::PrintToFile(bool bSignature, const char* format, va_list args) const
{
	//Open the log file for editing
	FILE* pFile = 0;

	if (!m_sFile.empty())
		pFile = fopen(m_sFile.c_str(), "a+");


	if (!pFile)
	{
		PrintToConsoleF("Unable to open Log File (%s)", m_sFile.c_str());
		return;
	}

	//Print the formatted string to the log file
	if (bSignature)
		fprintf(pFile, m_sSignature.c_str());

	vfprintf(pFile, format, args);
	fprintf(pFile, "\n");
	//Close the file after editing
	fclose(pFile);
}

void CLog::PrintToConsoleF(bool bSignature, const char* format, ...) const
{
	va_list args;
	va_start(args, format);
	PrintToConsole(bSignature, format, args);
	va_end(args);
}

void CLog::PrintToConsole(bool bSignature, const char* format, va_list args) const
{
	if (bSignature)
		printf(m_sSignature.c_str());

	vprintf(format, args);
	printf("\n");
}

void CLog::GetSignature(const void* src_obj, unsigned int level)
{
	char t[50] = { 0 };
	time_t temp;
	time(&temp);
	strftime(t, sizeof(t), "%T", localtime(&temp));

	char buffer[256] = { 0 };
	//[DATE][OBJ ADDRESS] | (level)[LEVEL_NAME] | --: LOG_MESSAGE
	if (src_obj)
		sprintf(buffer, "[%s][%p] \t| (%i)%s \t|: ", t, src_obj, level, GetLLString(level));
	else
		sprintf(buffer, "[%s][(null)] \t| (%i)%s \t|: ", t, level, GetLLString(level));

	m_sSignature = buffer;
}

const char* CLog::GetLLString(unsigned int LL)
{
	switch (LL)
	{
	case DEBUGLOG_LEVEL_DEBUG:
		return DEBUGLOG_DEBUG;
	case DEBUGLOG_LEVEL_INFO:
		return DEBUGLOG_INFO;
	case DEBUGLOG_LEVEL_ERROR:
		return DEBUGLOG_ERROR;
	case DEBUGLOG_LEVEL_WARNING:
		return DEBUGLOG_WARNING;
	case DEBUGLOG_LEVEL_FATAL:
		return DEBUGLOG_FATAL;
	case DEBUGLOG_LEVEL_ALL:
		return DEBUGLOG_ALL;
	}

	return "UNKNOWN";
}

unsigned int CLog::GetLLIndex(const char* str)
{
	for (int i = 0; i < DEBUGLOG_LEVEL_ALL + 1; i++)
	{
		if (!strcmp(str, GetLLString(i)))
			return i;
	}

	return DEBUGLOG_LEVEL_ALL + 1;
}

void CLog::OpenClean(bool b)
{
	m_bClear = b;
}

void Log(const void * src_obj, unsigned int level, const char * format, ...)
{
	va_list args;
	va_start(args, format);
	_LOG.Log(src_obj, level, format, args);
	va_end(args);
}
