#include <string>
#include <iostream>
#include "run-length.h"
#include "dictionary.h"
#include "tests.h"

#define REPS 100

int main (int argc, char *argv[])
{
    std::string mode;
    std::string file;
    if (argc < 3) {
        printf("Error: not enough parameters\n");
        exit(1);
    }

    mode = (std::string) argv[1];
    file = (std::string) argv[2];
    Timer time ;

    if (mode == "-rc") {
        time.start();
        rle::compress(file);
        time.stop();
        compression_ratio(file, file + ".rle");
        std::cout << "Time: " << time.get_time() << " microseconds\n";
    }
    else if (mode == "-rd") {
        time.start();
        rle::decompress(file);
        time.stop();
        std::cout << "Time: " << time.get_time() << " microseconds\n";
    }
    else if (mode == "-dc") {
        time.start();
        dict::compress(file);
        time.stop();
        compression_ratio(file, file + ".dc");
        std::cout << "Time: " << time.get_time() << " microseconds\n";
    }
    else if (mode == "-dd") {
        time.start();
        dict::decompress(file, (std::string)argv[3]);
        time.stop();
        std::cout << "Time: " << time.get_time() << " microseconds\n";
    }
    else {
        printf("Error: wrong option\n");
    }

    return 0;
}
