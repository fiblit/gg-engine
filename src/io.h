#ifndef IO_H
#define IO_H

#include <string>
#include <experimental/optional>

std::experimental::optional<const std::string> read_file(const std::string path);

#endif//IO_H
