#ifndef __TIMER_H__
#define __TIMER_H__

#include <windows.h>

class Timer
{
	HANDLE mThread;
	DWORD_PTR mpProcessAffinityMask;
	DWORD_PTR mpSystemAffinityMask;
	LARGE_INTEGER mFrequency;
	LARGE_INTEGER mStart;
	LARGE_INTEGER mEnd;
public:
	Timer();

	void Start();
	double GetTime();
};

#endif