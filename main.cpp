#include <string>
#include "run-length.h"

int main (int argc, char *argv[])
{
    std::string file;
    if (argc > 1)
        file = argv[1];
    else
        file = "test.txt";

    // Testing (de)compress functions for RLE (manual change)
    rle::compress(file);
    // rle::decompress(file);
    return 0;
}
