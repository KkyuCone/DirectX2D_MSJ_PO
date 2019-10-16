#include "Time.h"

Time::Timer Time::m_Timer;

///////////////////////////////////////// Timer
Time::Timer::Timer()
	: Count(), Current_Time(), Previous_Time(), deltaTime(0.0f)
	, m_ElapseTime(0.0f), m_FPSCount(0), m_FPS(0)
{
	QueryPerformanceFrequency(&Count);
	QueryPerformanceCounter(&Current_Time);

	Previous_Time = Current_Time;
}

Time::Timer::~Timer()
{
}


void Time::Timer::Update()
{
	float a = deltaTime;
	QueryPerformanceCounter(&Current_Time);
	deltaTime = (Current_Time.QuadPart - Previous_Time.QuadPart) / (float)Count.QuadPart;
	Previous_Time = Current_Time;
}

void Time::Timer::Timer_FPS()
{
	++m_FPSCount;
	m_ElapseTime += deltaTime;

	if (m_ElapseTime >= 1.0f)
	{
		m_FPS = m_FPSCount;
		m_FPSCount = 0;
		m_ElapseTime = 0.0f;
	}
}


//

Time::Time()
{
}


Time::~Time()
{
}

void Time::Update()
{
	m_Timer.Update();
	m_Timer.Timer_FPS();
}
