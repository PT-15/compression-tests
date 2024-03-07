#pragma once

#include <ratio>
#include <string>
#include <chrono>

void compression_ratio (const std::string original_s, const std::string compressed_s);

class Timer {
    private:
        typedef std::chrono::time_point<std::chrono::high_resolution_clock> chrono_time ;

        chrono_time _start;
        chrono_time _stop;

    public:
        Timer();
        ~Timer();
        void start();
        void stop();
        std::chrono::microseconds::rep get_time();
};
