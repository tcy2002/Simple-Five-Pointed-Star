#pragma once

#include <iostream>
#include <wingdi.h>
#include "GL/glew.h"

class BmpLoader {
private:
    int channel;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

public:
    char *textureData;
    int width, height;
    int format;

    explicit BmpLoader(const char *file) {
        long offset = 0;
        memcpy(&bfh, file, sizeof(BITMAPFILEHEADER));
        if (bfh.bfType != 0x4D42) {
            std::cout << "Invalid Texture Format: Type" << std::endl;
            exit(EXIT_FAILURE);
        }

        offset += sizeof(BITMAPFILEHEADER);
        memcpy(&bih, file + offset, sizeof(BITMAPINFOHEADER));
        width = bih.biWidth;
        height = bih.biHeight;
        if (bih.biBitCount == 24) {
            channel = 3;
            format = GL_RGB;
        }
        else if (bih.biBitCount == 32) {
            channel = 4;
            format = GL_RGBA;
        }
        else {
            std::cout << "CRASH:Invalid Texture Format: Channel" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (bih.biSizeImage == 0)
            bih.biSizeImage = width * height * channel;

        textureData = new char[bih.biSizeImage];

        offset = (long)bfh.bfOffBits;
        memcpy(textureData, file + offset, bih.biSizeImage);

        unsigned char temp;
        for (int i = 0; i < bih.biSizeImage; i += channel) {
            temp = textureData[i];
            textureData[i] = textureData[i + 2];
            textureData[i + 2] = temp;
        }
    }

    ~BmpLoader() {
        delete[] textureData;
    }
};