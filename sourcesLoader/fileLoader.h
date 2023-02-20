#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

class FileLoader {
private:
    size_t size;

public:
    char *buf;

    explicit FileLoader(const char *name): size(0), buf(nullptr) {
        std::ifstream file(name);
        if (!file.is_open()) {
            std::cout << "Failed to open file!" << std::endl;
            exit(EXIT_FAILURE);
        }

        std::ostringstream str;
        char ch;
        while (str && file.get(ch) && ++size)
            str << ch;

        buf = new char[size + 1];
        memcpy(buf, str.str().c_str(), size);
        buf[size] = 0;
    }

    ~FileLoader() {
        delete[] buf;
    }
};