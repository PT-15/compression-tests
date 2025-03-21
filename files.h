#pragma once

#include <string>
#include <sys/stat.h>

#define BUF_SIZE 1024

class File {
    private:
        int _fd = 0;
        long unsigned int _pos = 0;
        unsigned char _bits_left = 8;
        unsigned char _mask = 128;
        unsigned char _buffer[BUF_SIZE] = {0};
        struct stat _file_stats;

        void refill_buffer();
        void write_to_file (int bytes);
        void bits_to_char(unsigned char length, unsigned char key);

    public:
        File (const std::string name, bool input);
        ~File();
        void close_file();

        void move_to_start();

        char read_char ();
        char read_bit ();
        int read_int();

        void write_char (const char& info);
        void write_bits (std::pair<long long,char> code);
        void write_int (const int info);

        void flush();

        int get_size();
};

