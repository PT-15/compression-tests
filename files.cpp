#include "files.h"

#include <fcntl.h>

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
