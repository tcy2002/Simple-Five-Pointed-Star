cmake_minimum_required(VERSION 3.22)
project(hw1-ulti)

set(CMAKE_CXX_STANDARD 14)

set(EXECUTABLE_OUTPUT_PATH ..)
set(DCMAKE_BUILD_TYPE Release)

link_directories(env/lib/x64)
include_directories(env/include sourcesLoader shader texture)
include_directories(.)

add_executable(
        hw1-ulti
        main.cpp
        data.h
        config.h
        sourcesLoader/shaderLoader.h
        sourcesLoader/fileLoader.h
        sourcesLoader/bmpLoader.h
        sourcesLoader/textureLoader.h
        shader/shaderTextBuf.h
        texture/textureTextBuf.h
)

target_link_libraries(
        hw1-ulti
        glew32.lib
        libfreeglut.a
        libopengl32.a
)
