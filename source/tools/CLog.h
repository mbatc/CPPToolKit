#pragma once

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdarg.h>

using namespace std;

//DEBUG LOG LEVELS
#define DEBUGLOG_LEVEL_ALL 	0x0006
#define DEBUGLOG_LEVEL_DEBUG 	0x0004
#define DEBUGLOG_LEVEL_WARNING 	0x0003
#define DEBUGLOG_LEVEL_ERROR 	0x0002
#define DEBUGLOG_LEVEL_FATAL 	0x0001
#define DEBUGLOG_LEVEL_INFO	0x0005

#define DEBUGLOG_ALL		"ALL"
#define DEBUGLOG_WARNING 	"WARN"
#define DEBUGLOG_ERROR 		"ERROR"
#define DEBUGLOG_DEBUG 		"DEBUG"
#define DEBUGLOG_FATAL 		"FATAL"
#define DEBUGLOG_INFO		"INFO"

#define DEBUGLOG_DEFAULT_FILE "Logs\\default.txt"

class CLog
{
public:
	CLog(const char* file = DEBUGLOG_DEFAULT_FILE);
	~CLog();

	//If true then logs will be printed to the command window
	void CMDLog(bool cmdLog);

	//Set what type of messages to log
	void SetLogLevel(unsigned int level);
	//Set the file to save logs to (default is "Logs\\default.txt")
	void SetLogFile(string file);

	//Creates an entry in the debug log
	void Log(const void* src_obj, unsigned int level, const char* format, ...);
	void Log(const void* src_obj, unsigned int level, const char* format, va_list args);

	//Get the Log Level String definition
	const char* GetLLString(unsigned int LL);
	//Get the lof level index from string definition
	unsigned int GetLLIndex(const char* str);
	
	//set whether to append debug log or clear it
	void OpenClean(bool b);
private:
	//Print Log Header
	void PrintHeader();
	//Print Log Footer
	void PrintFooter();

	//Creates the log signature and stores it in the 'm_sSignature' member
	void GetSignature(const void* src_obj, unsigned int level);

	//Prints the formatted string to the log file
	void PrintToFile(bool bPrintSignature, const char* format, va_list args) const;
	void PrintToFileF(bool bPrintSignature, const char* format, ...) const;
	//Prints the formatted string to the console
	void PrintToConsole(bool bPrintSignature, const char* format, va_list args) const;
	void PrintToConsoleF(bool bPrintSignature, const char* format, ...) const;
	string		m_sFile;
	string		m_sSignature;

	bool		m_bPrint_time;
	bool 		m_bCmd_log;
	unsigned int 	m_log_level;

	char 		m_log_time[50];

	bool		m_bClear;
};

extern CLog _LOG;

void Log(const void* src_obj, unsigned int level, const char* format, ...);