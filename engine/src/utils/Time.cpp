#include "utils/Time.hpp"

int Time::FrameRate()
{
	static auto last_time = Clock::now();
	static int frame_count = 0;
	static int current_fps = 0;

	auto now = Clock::now();
	std::chrono::duration<float> elapsed = now - last_time;

	frame_count++;

	if (elapsed.count() >= 1.0f) {
		current_fps = frame_count;
		std::cout << "FPS:" << current_fps << "\n";
		frame_count = 0;
		last_time = now;
	}


	return current_fps;
}

float Time::FrameTime()
{
	static auto last_time = Clock::now();

	auto now = Clock::now();

	std::chrono::duration<float> elapsed = now - last_time;

	last_time = now;

	return elapsed.count();
}
