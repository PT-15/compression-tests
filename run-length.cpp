#include "run-length.h"

#include <cstdio>
#include <unistd.h>

#include "files.h"

#define BUF_SIZE 1024

void rle::compress(const std::string file)
{
    int in_fd, out_fd;
    open_files(file, file + ".rle", in_fd, out_fd);

    char prev = next_char(in_fd);
    if (prev == EOF && prev == '\000') {
        return;
    }
    char cnt = 1;
    char current;
    while ((current = next_char(in_fd)) != EOF && current != '\000') {
        if (current == prev)
            cnt++;
        else {
            write_char(out_fd, prev);
            write_char(out_fd, cnt);
            cnt = 1;
            prev =  current;
        }
    }
    write_char(out_fd, prev);
    write_char(out_fd, cnt, true);

    close(in_fd);
    close(out_fd);
}

void rle::decompress(const std::string file)
{
    int in_fd, out_fd;
    open_files(file, file.substr(0, file.size()-4), in_fd, out_fd);

    char value;
    char cnt;
    while ((value = next_char(in_fd)) != EOF && value != '\000') {
        cnt = next_char(in_fd);

        for (int i = 0; i < (int)cnt; i++) {
            write_char(out_fd, value);
        }
    }
    write_char(out_fd, '\000', true);

    close(in_fd);
    close(out_fd);
}
