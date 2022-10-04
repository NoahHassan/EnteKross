#pragma once

#include <chrono>

using namespace std::chrono;

class Timer
{
public:
	Timer() noexcept
	{
		last = steady_clock::now();
	}
public:
	float Mark() noexcept
	{
		steady_clock::time_point now = steady_clock::now();
		duration<float> duration = now - last;

		last = now;
		return duration.count();
	}
	float Peek() const noexcept
	{
		steady_clock::time_point now = steady_clock::now();
		duration<float> duration = now - last;

		return duration.count();
	}
private:
	std::chrono::steady_clock::time_point last;
};