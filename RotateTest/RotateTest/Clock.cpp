#include "Clock.h"
#include <windows.h>


Clock::Clock()
{
}


Clock::~Clock()
{
}

bool Clock::Init()
{
	LARGE_INTEGER qwTicksPerSec, qwTime;

	isQPF = (bool)(QueryPerformanceFrequency(&qwTicksPerSec) != 0);

	if (!isQPF)
	{
		return false;
	}
	qpfTicksPerSec = qwTicksPerSec.QuadPart;

	QueryPerformanceCounter(&qwTime);
	lastElapsedTime = qwTime.QuadPart;
	integralTime = 0.f;
	return true;
}

void Clock::ProcessTime()
{
	if (!isQPF)
	{
		return;
	}

	LARGE_INTEGER qwTime;
	QueryPerformanceCounter(&qwTime);

	elapsedTime = (float)
		((double)(qwTime.QuadPart - lastElapsedTime)
			/ (double)qpfTicksPerSec);

	lastElapsedTime = qwTime.QuadPart;
}

bool Clock::isOneFrame(float *dt)
{
	integralTime += GetElapsedTime();
	*dt = integralTime;
	if (integralTime >= 0.016667)
	{
		integralTime = 0.f;
		return true;
	}
	return false;
}
