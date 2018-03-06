#include "CTimer.h"

#include <iostream>

using namespace std;

CTimer::CTimer()
	:
	m_nTimersSet(0),
	m_bTimerSet(false)
{}

CTimer::~CTimer()
{}

void CTimer::Start()
{
	if (!m_start)
	{
		m_startClock = clock();
		m_start = true;
	}
}

void CTimer::End()
{
	if (m_start)
	{
		m_endClock = clock();
		m_start = false;
		m_duration = m_endClock - m_startClock;
		m_nTimersSet++;
		m_bTimerSet = true;
	}
}

double CTimer::GetTime()
{
	if (m_bTimerSet)
		return m_duration/CLOCKS_PER_SEC;
	else
		return 0.0f;
}

void CTimer::ResetNTimers()
{
	m_nTimersSet = 0;
}

int CTimer::GetNTimers()
{
	return m_nTimersSet;
}
