#pragma once

#include <ctime>

class CTimer
{
public:
	CTimer();
	~CTimer();

	void Start();
	void End();
	double GetTime();
	void ResetNTimers();
	int GetNTimers();

private:
	bool m_start;
	bool m_bTimerSet;
	
	int m_nTimersSet;

	clock_t m_startClock;
	clock_t m_endClock;
	double m_duration;
};