#include "io.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace std::experimental;

optional<const string> read_file(const string path) {
    ifstream file(path, ios::in | ios::binary);
    if (!file) {
        cerr << "gg! Failed to open file " << path << "\n";
        return nullopt;
    }

    file.seekg(0, ios::end);
    string content(static_cast<size_t>(file.tellg()), '\0');
    file.seekg(0, ios::beg);
    file.read(&content[0], static_cast<long int>(content.size()));
    if (!file) {
        cerr << "gg! Failed to read entire file " << path << "\n";
        return nullopt; 
    }

    return content;
}
