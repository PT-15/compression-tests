#include "files.h"

#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 1024

void open_files (const std::string in_name, const std::string out_name, int &in_fd, int &out_fd)
{
    in_fd = open(in_name.c_str(), O_RDONLY);
    if (in_fd < 0) {
        std::perror("Couldn't open input file");
        exit(1);
    }

    out_fd = open(out_name.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (out_fd < 0) {
        std::perror("Couldn't open output file");
        exit(1);
    }
}

char next_char (int fd)
{
    static long unsigned int pos = BUF_SIZE;
    static char buffer [BUF_SIZE];

    if (pos >= BUF_SIZE) {
        read(fd, buffer, BUF_SIZE);
        pos = 0;
    }

    return buffer[pos++];
}

void write_char (int fd, const char& info, bool flush)
{
    static long unsigned int pos = 0;
    static char buffer [BUF_SIZE];

    if (pos >= BUF_SIZE) {
        write(fd, &buffer, BUF_SIZE);
        pos = 0;
    }

    if (info != '\000') {
        buffer[pos] = info;
        pos++;
    }

    if (flush) {
        write(fd, &buffer, pos);
    }
}
