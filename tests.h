#pragma once

#include <string>
#include <chrono>

void compression_ratio (const std::string original_s, const std::string compressed_s);

class Timer {
    private:
        typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> chrono_time ;

        chrono_time _start;
        chrono_time _stop;

    public:
        Timer();
        void start();
        void stop();
        void print_time();
};
