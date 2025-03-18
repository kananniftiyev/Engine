#pragma once

#include <chrono>
#include <iostream>
#include <vector>

using Clock = std::chrono::high_resolution_clock;


class Time
{
public:
	static int FrameRate();
	static float FrameTime();

private:

};
