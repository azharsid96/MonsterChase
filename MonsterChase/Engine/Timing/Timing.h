#pragma once
#include "Windows.h"

class Timing
{
public:
	LARGE_INTEGER GetCurrentCPUCounter();
	float GetCPUCounterFrequency_ms();
	float CalcLastFrameTime_ms();

private:
};