#include "run-length.h"

#include <fcntl.h>
#include <unistd.h>

#include "files.h"

void rle::compress(std::string file)
{
    int in_fd, out_fd;
    open_files(file, file + ".rle", in_fd, out_fd);

    char current;
    char cnt = 1;
    char prev;
    if (read(in_fd, &prev, 1) > 0) { // File not empty
        while (read(in_fd, &current, 1) > 0) {
            if (current == prev) {
                cnt++;
                continue;
            }
            write(out_fd, &prev, 1);
            write(out_fd, &cnt, 1);
            cnt = 1;
            prev =  current;
        }
    }

    close(in_fd);
    close(out_fd);
}

void rle::decompress(std::string file)
{
    int in_fd, out_fd;
    open_files(file, file.substr(0, file.size()-4), in_fd, out_fd);

    char current;
    char cnt;

    while (read(in_fd, &current, 1) > 0) {
        read(in_fd, &cnt, 1);

        for (int i = 0; i < (int) cnt; i++) {
            write(out_fd, &current, 1);
        }
    }

    close(in_fd);
    close(out_fd);
}
