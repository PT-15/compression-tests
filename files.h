#pragma once

#include <string>

void open_files (const std::string in_name, const std::string out_name, int &in_fd, int &out_fd);

char next_char (int fd);
// If info is '\000' it only flushes the output to the file
void write_char (int fd, const char& info, bool flush = false);
