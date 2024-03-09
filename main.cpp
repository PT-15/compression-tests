#include <string>
#include "run-length.h"
#include "dictionary.h"
#include "compare-methods.h"

#define REPS 100

int main (int argc, char *argv[])
{
    if (argc < 3) {
        printf("HOW TO USE:\n");
        printf("\texecutable -r  [file/file.rle] :"
                " compress/decompress using run length encoding\n");
        printf("\texecutable -d [file/file.dc] :"
                " compress/decompress using dictionary encoding\n");
        printf("\texecutable -t file [file2 file3...] :"
                " compare coding algorithms\n");
        exit(0);
    }

    std::string mode = (std::string) argv[1];

    if (mode == "-r") {
        for (int i = 2; i < argc; i++) {
            std::string file = (std::string) argv[i];
            std::string extension = file.substr(file.size()-4, file.size());

            if (extension == ".rle")
                rle::decompress(file);
            else
                rle::compress(file);
        }
    }
    else if (mode == "-d") {
        for (int i = 2; i < argc; i++) {
            std::string file = (std::string) argv[i];
            std::string extension = file.substr(file.size()-3, file.size());

            if (extension == ".dc")
                dict::decompress(file);
            else {
                dict::compress(file);
                printf("Do not erase the .dic file,"
                        " you will need it to decompress\n");
            }
        }
    }
    else if (mode == "-t") {
        for (int i = 2; i < argc; i++) {
            compare_methods(argv[i]);
        }
    }
    else {
        printf("Error: not a valid parameter\n");
    }

    return 0;
}
