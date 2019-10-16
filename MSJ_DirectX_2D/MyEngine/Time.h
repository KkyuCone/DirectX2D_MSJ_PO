#pragma once
#include <Windows.h>

class Time
{
public:
	friend class Window;

private:
	class Timer
	{
	public:
		friend Time;

	private:
		LARGE_INTEGER Count;
		LARGE_INTEGER Current_Time;
		LARGE_INTEGER Previous_Time;

		float deltaTime;

		//FPS
		float m_ElapseTime;		// 경과시간
		int m_FPSCount;
		int m_FPS;

	public:
		Timer();
		~Timer();

	private:
		void Update();
		void Timer_FPS();
	};

private:
	static Timer m_Timer;

public:
	static inline float DeltaTime()
	{
		return m_Timer.deltaTime;
	}

	static inline int FPS()
	{
		return m_Timer.m_FPS;
	}

public:
	static void Update();

public:
	Time();
	~Time();
};

