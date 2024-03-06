#include "files.h"

#include <fcntl.h>
#include <unistd.h>

#define KEY first
#define LEN second

File::File (const std::string name, bool input)
{
    if (input) {
        _fd = open(name.c_str(), O_RDONLY);
        _pos = BUF_SIZE;
    }
    else
        _fd = open(name.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);

    if (_fd < 0) {
        std::perror("Couldn't open file");
        exit(1);
    }
    fstat(_fd, &_file_stats);
}

void File::refill_buffer()
{
    read(_fd, _buffer, BUF_SIZE);
    _pos = 0;
}

void File::move_to_start()
{
    write_to_file(_pos);
    lseek(_fd, 0, SEEK_SET);
    _pos = 0;
}


char File::read_char()
{
    if (_pos >= BUF_SIZE)
        refill_buffer();

    return _buffer[_pos++];
}

char File::read_bit ()
{
    if (_pos >= BUF_SIZE)
        refill_buffer();

    if (_buffer[_pos] == EOF || _buffer[_pos] == '\000') {
        return -1;
    }

    bool bit = _buffer[_pos] & _mask;
    _mask >>= 1;
    if (_mask == 0) {
        _mask = 128;
        _pos++;
    }

    return bit;
}

int File::read_int()
{
    if (_pos >= BUF_SIZE)
        refill_buffer();
        
    int ans = 0;
    char bytes = sizeof(int);

    while (bytes--) {
        ans += ((int)_buffer[_pos] << 8*(bytes-1));
        _pos++;
    }

    return ans;
}

void File::write_to_file (int bytes)
{
    write(_fd, &_buffer, bytes);
    _pos = 0;
}

void File::write_char (const char& info)
{
    if (_pos >= BUF_SIZE)
        write_to_file(BUF_SIZE);

    _buffer[_pos] = info;
    _pos++;
}
void File::write_bits (std::pair<int,char> code)
{
    if (_pos >= BUF_SIZE)
        write_to_file(BUF_SIZE);

    while (code.LEN > 0) {
        int offset = code.LEN - _bits_left;
        if (offset < 0) { // Fits in current byte
            _buffer[_pos] += code.KEY << abs(offset);
            _bits_left -= code.LEN;
            code.LEN = 0;
        }
        else {
            _buffer[_pos] += code.KEY >> offset;
            code.LEN -= _bits_left;
            _bits_left = 8;
            _pos++;
        }
    }
}

void File::write_int (const int info)
{
    if (_pos >= BUF_SIZE)
        write_to_file(BUF_SIZE);

    char bytes = sizeof(int);

    while (bytes--) {
        _buffer[_pos++] = info >> (8*bytes);

        if (_pos >= BUF_SIZE)
            write_to_file(BUF_SIZE);
    }
}

void File::flush()
{
    write_to_file(_pos);
}

int File::get_size()
{
    return _file_stats.st_size; // Size in bytes
}

File::~File()
{
    close(_fd);
}
