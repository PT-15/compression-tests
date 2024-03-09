#include "compare-methods.h"

#include <iostream>
#include <pstl/glue_algorithm_defs.h>
#include <string>

#include "run-length.h"
#include "dictionary.h"
#include "timer.h"
#include "files.h"

#define REPS 100

typedef std::chrono::microseconds::rep msec;

msec get_time (void (*func) (const std::string&), std::string file)
{
    Timer timer;
    msec time = 0;
    for (int i = 0; i < REPS; i++) {
        timer.start();
        func(file);
        timer.stop();
        time += timer.get_time();
    }
    return time / REPS;
}

void compare_compression (std::string file)
{
    rle::compress(file);
    dict::compress(file);

    File original (file, true);
    File rle_compressed (file + ".rle", true);
    File dict_compressed (file + ".dc", true);
    File dictionary (file + ".dic", true);

    std::cout << original.get_size() << " bytes : original file\n";
    std::cout << rle_compressed.get_size() << " bytes : run length encoding compressed\n";
    int dict_total = dict_compressed.get_size() + dictionary.get_size();
    std::cout << dict_total << " bytes : dictionary compressed (" <<
                dict_compressed.get_size() << " compressed file + " <<
                dictionary.get_size() << " dictionary)\n";

    std::cout << "\n";
    float rle_cr = (float)original.get_size() / (float)rle_compressed.get_size();
    float dict_cr = (float)original.get_size() / (float)dict_total;
    std::cout << rle_cr << " : rle compression ratio\n";
    std::cout << dict_cr << " : dictionary compression ratio\n";

    original.close_file();
    rle_compressed.close_file();
    dict_compressed.close_file();
    dictionary.close_file();
}

void compare_times (std::string file)
{
    msec rle_comp_t= get_time(rle::compress, file);
    msec rle_decomp_t= get_time(rle::decompress, file + ".rle");
    msec dict_comp_t= get_time(dict::compress, file);
    msec dict_decomp_t= get_time(dict::decompress, file + ".dc");

    std::cout << "Compression:\n";
    std::cout << "\tRun length encoding: " << rle_comp_t << " milisecond\n";
    std::cout << "\tDictionary encoding: " << dict_comp_t << " milisecond\n";
    std::string fastest = rle_comp_t > dict_comp_t ? "Dictionary encoding" : "Run length encoding";
    std::cout << fastest << " was " <<
        std::max(rle_comp_t, dict_comp_t) / std::min(rle_comp_t, dict_comp_t) <<
        " times faster\n";

    std::cout << "Decompression:\n";
    std::cout << "\tRun length encoding: " << rle_decomp_t << " milisecond\n";
    std::cout << "\tDictionary encoding: " << dict_decomp_t << " milisecond\n";
    fastest = rle_decomp_t > dict_decomp_t ? "Dictionary encoding" : "Run length encoding";
    std::cout << fastest << " was " <<
        std::max(rle_decomp_t, dict_decomp_t) / std::min(rle_decomp_t, dict_decomp_t) <<
        " times faster\n";
}

void compare_methods (char *name)
{
    std::string file = (std::string)name;
    std::cout << "--- " << file << " ---\n";
    compare_compression(file);
    std::cout << "\n";
    compare_times(file);
}
