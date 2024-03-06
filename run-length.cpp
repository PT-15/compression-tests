#include "run-length.h"

#include <cstdio>
#include <unistd.h>

#include "files.h"

#define BUF_SIZE 1024

void rle::compress(const std::string file)
{
    File input (file, true);
    File output (file + ".rle", false);

    char prev = input.read_char();
    if (prev == EOF && prev == '\000') {
        return;
    }
    char cnt = 1;
    char current;
    while ((current = input.read_char()) != EOF && current != '\000') {
        if (current == prev)
            cnt++;
        else {
            output.write_char(prev);
            output.write_char(cnt);
            cnt = 1;
            prev =  current;
        }
    }
    output.write_char(prev);
    output.write_char(cnt);
    output.flush();
}

void rle::decompress(const std::string file)
{
    File input (file, true);
    File output (file.substr(0, file.size()-4), false);

    char element;
    char cnt;
    while ((element = input.read_char()) != EOF && element != '\000') {
        cnt = input.read_char();

        for (int i = 0; i < (int)cnt; i++) {
            output.write_char(element);
        }
    }
    output.flush();
}
