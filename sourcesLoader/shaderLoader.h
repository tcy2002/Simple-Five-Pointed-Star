#pragma once

#include <iostream>
#include "GL/glew.h"

#include "fileLoader.h"
#include "config.h"

void compileShaderAndCheck(int shader) {
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Shader Compile Error: \n" << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
}

int loadVertexShader(const char *filename) {
    FileLoader vs(filename);
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vs.buf, nullptr);
    compileShaderAndCheck(vertexShader);
    return vertexShader;
}

int createVertexShader(const char *buf) {
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &buf, nullptr);
    compileShaderAndCheck(vertexShader);
    return vertexShader;
}

int loadFragmentShader(const char *filename) {
    FileLoader fs(filename);
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fs.buf, nullptr);
    compileShaderAndCheck(fragmentShader);
    return fragmentShader;
}

int createFragmentShader(const char *buf) {
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &buf, nullptr);
    compileShaderAndCheck(fragmentShader);
    return fragmentShader;
}

void linkProgramAndCheck(int program) {
    glLinkProgram(program);
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "Program Compile Error: \n" << infoLog << std::endl;
    }
}

int createShaderProgram(int vertexShader, int fragmentShader) {
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    linkProgramAndCheck(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}
