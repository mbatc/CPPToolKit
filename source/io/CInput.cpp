#include "CInput.h"
#include "../tools/CLog.h"


CInput::CInput()
{
	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "Input Class Initialised");
}

CInput::~CInput()
{}

string CInput::GetCMDLine()
{
	string str = "";
	char buf[2] = { 0 };
	do
	{
		buf[0] = getchar();

		if (buf[0] == '\0' || buf[0] == '\n' || buf[0] == '\r')
			break;

		str += buf;
	} while (true);

	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "Getting User Input: %s", str.c_str());
	return str;
}