#include "Timing.h"

float Timing::CalcLastFrameTime_ms()
{
	static long g_LastFrameStartTick = 0;
	float g_LastFrameTime_ms;

	//Grab the CPU tick counter
	const long currentTick = static_cast<long>(GetCurrentCPUCounter().QuadPart);

	if (g_LastFrameStartTick) {
		// How many ticks have elapsed since we last did this
		const long elapsedTicks = (currentTick - g_LastFrameStartTick);

		// Calculate the frame time - converting ticks to ms.
		g_LastFrameTime_ms = elapsedTicks / GetCPUCounterFrequency_ms();
	}
	else {
		g_LastFrameTime_ms = 13.3f; // Assume a 60Hz frame for first call.
	}
	// Note start of this frame
	g_LastFrameStartTick = currentTick;

	return g_LastFrameTime_ms;
}

LARGE_INTEGER Timing::GetCurrentCPUCounter()
{
	LARGE_INTEGER ticks;
	QueryPerformanceCounter(&ticks);
	
	//const long CPUCounter = ticks.QuadPart;

	return ticks;
}

float Timing::GetCPUCounterFrequency_ms()
{
	LARGE_INTEGER ticksPerSecond;

	QueryPerformanceFrequency(&ticksPerSecond);

	//const long CPUFrequency = ticksPerSecond.QuadPart;

	return ticksPerSecond.QuadPart;
}


