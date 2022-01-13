#pragma once
#include <chrono>
//#include <>

struct FrameTimeInfo
{
	float duration;
	std::chrono::time_point<std::chrono::steady_clock> timepoint;
};

#define uniform(name) #name, &##name