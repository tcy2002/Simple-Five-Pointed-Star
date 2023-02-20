#pragma once

#include <GL/glew.h>
#include "bmpLoader.h"
#include "fileLoader.h"

unsigned int loadTexture(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        std::cout << "Missing File" << std::endl;
        exit(EXIT_FAILURE);
    }
    FileLoader bf(filename);
    BmpLoader bl(bf.buf);
    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, bl.format, bl.width, bl.height,
                 0, bl.format, GL_UNSIGNED_BYTE, bl.textureData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

unsigned int createTexture(const char *buf) {
    BmpLoader bl(buf);
    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, bl.format, bl.width, bl.height,
                 0, bl.format, GL_UNSIGNED_BYTE, bl.textureData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}