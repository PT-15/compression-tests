#include "timer.h"

#include <chrono>

/////////////////
// TIMER CLASS //
/////////////////

Timer::Timer() {}

void Timer::start()
{
    _start = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
    _stop = std::chrono::high_resolution_clock::now();
}

std::chrono::microseconds::rep Timer::get_time()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(_stop - _start).count();
}

Timer::~Timer() {}

