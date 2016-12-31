#include "Timer.h"

Timer::Timer()
{
	mThread = GetCurrentThread();
	GetProcessAffinityMask(GetCurrentProcess(), &mpProcessAffinityMask, &mpSystemAffinityMask);
	SetThreadAffinityMask(mThread, 1);
	QueryPerformanceFrequency(&mFrequency);
	SetThreadAffinityMask(mThread, mpProcessAffinityMask);
}
void Timer::Start()
{
	SetThreadAffinityMask(mThread, 1);
	QueryPerformanceCounter(&mStart);
	SetThreadAffinityMask(mThread, mpProcessAffinityMask);
}
double Timer::GetTime()
{
	SetThreadAffinityMask(mThread, 1);
	QueryPerformanceCounter(&mEnd);
	SetThreadAffinityMask(mThread, mpProcessAffinityMask);
	
	return (double)(mEnd.QuadPart - mStart.QuadPart) / (double)mFrequency.QuadPart;
}