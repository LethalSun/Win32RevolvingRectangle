#pragma once
#include <windows.h>
class Clock
{
public:
	Clock(void);
	virtual ~Clock(void);

	inline float GetElapsedTime() const
	{
		return elapsedTime;
	};

	bool Init();
	void ProcessTime();

	bool isOneFrame(float*);
private:
	bool		isQPF;
	float		elapsedTime;
	float		integralTime;
	LONGLONG	qpfTicksPerSec;
	LONGLONG	lastElapsedTime;
};


