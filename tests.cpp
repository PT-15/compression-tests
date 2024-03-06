#include "tests.h"

#include <chrono>
#include <iostream>

#include "files.h"

void compression_ratio (const std::string original_s, const std::string compressed_s)
{
    File original (original_s, true);
    File compressed (compressed_s, true);
    printf("Original size: %d\n", original.get_size());
    printf("Compressed size: %d\n", compressed.get_size());
    printf("Compression ratio: %f\n", (float)original.get_size() / (float)compressed.get_size());
}

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

void Timer::print_time()
{
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(_stop - _start);
    std::cout << "Time: " << duration.count() << " microseconds\n";
}

