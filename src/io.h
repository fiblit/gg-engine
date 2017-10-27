#ifndef IO_H
#define IO_H

#include <string>
#include <experimental/optional>

std::experimental::optional<const std::string> read_file(const std::string path);

struct Image {
    int width;
    int height;
    int channels;
    unsigned char* bytes;
};

std::experimental::optional<Image> read_image(const std::string image);

#endif//IO_H
