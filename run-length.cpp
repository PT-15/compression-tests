#include "run-length.h"

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

#include "files.h"

#define BUF_SIZE 1024

char next_char (int fd)
{
    static long unsigned int pos = BUF_SIZE;
    static char buffer [BUF_SIZE];
    // static std::vector<char> buffer (BUF_SIZE,0);

    if (pos >= BUF_SIZE) {
        int error = read(fd, buffer, BUF_SIZE);
        printf("Read %d bytes\n", error);
        pos = 0;
    }

    return buffer[pos++];
}

void write_char (int fd, const char& info, bool flush = false)
{
    static long unsigned int pos = 0;
    static char buffer [BUF_SIZE];

    if (pos >= BUF_SIZE) {
        int error = write(fd, &buffer, BUF_SIZE);
        printf("Wrote %d bytes\n", error);
        pos = 0;
    }

    if (info != '\000') {
        buffer[pos] = info;
        pos++;
    }

    if (flush) {
        int error = write(fd, &buffer, pos);
        printf("Wrote %d bytes\n", error);
    }
}

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
