#include "files.h"

#include <fcntl.h>
#include <unistd.h>

#include "debug.h"

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
    _pos++;
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

    // for (int i = 0; i < bytes; i++) {
    while (bytes--) {
        ans += ((int)_buffer[_pos] << (8*bytes));
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

void File::bits_to_char (unsigned char length, unsigned char key)
{
    if (_pos >= BUF_SIZE)
        write_to_file(BUF_SIZE);
    dbg::value("\tCode value", key);
    dbg::value("\tCode length", length);

    while (length > 0) {
        int offset = length - _bits_left;
        if (offset < 0) { // Fits in current byte
            dbg::bits("\tBuffer value", _buffer[_pos]);
            _buffer[_pos] += key << abs(offset);
            dbg::bits("\tValue written", key << abs(offset));
            dbg::bits("\tNew buffer value", _buffer[_pos]);
            _bits_left -= length;
            length = 0;
        }
        else {
            _buffer[_pos] += key >> offset;
            length -= _bits_left;
            _bits_left = 8;
            _pos++;
            _buffer[_pos] = 0; // Not valid if next byte is already in use
        }
    }
}

void File::write_bits (std::pair<int,char> code)
{
    int bytes = sizeof(int);
    
    const int key = code.first;
    const int length = code.second;
    dbg::msg("WRITING BITS");
    dbg::value("Received key", key);
    dbg::value("Received length", length);

    while (bytes--) {
        dbg::value("\tByte", bytes);
        dbg::bits("\tValue", key >> (8*bytes));
        if (bytes*8 < length) {
            bits_to_char(length, key >> (8*bytes));
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
