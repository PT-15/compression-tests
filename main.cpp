#include <string>
#include "run-length.h"
#include "dictionary.h"
#include "tests.h"

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
        time.print_time();
    }
    else if (mode == "-rd") {
        time.start();
        rle::decompress(file);
        time.stop();
        time.print_time();
    }
    else if (mode == "-dc") {
        time.start();
        dict::compress(file);
        time.stop();
        compression_ratio(file, file + ".dc");
        time.print_time();
    }
    else if (mode == "-dd") {
        time.start();
        dict::decompress(file, (std::string)argv[3]);
        time.stop();
        time.print_time();
    }

    return 0;
}
