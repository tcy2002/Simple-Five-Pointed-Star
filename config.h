/********************************************
 * 此文件定义了一些需要用到的常数、类型与工具函数
 *******************************************/

#pragma once

#include <iostream>
#include <cmath>

#define PI 3.141593

#define MAX_PARTICLES 1200

#define ORI_WIDTH 600
#define ORI_HEIGHT 600

enum DIRECT {D_FIXED, D_RAND};

typedef float Vector2f[2];
typedef float Vector3f[3];
typedef float Vector4f[4];

namespace color {
    Vector4f sjtuRed = {0.784f, 0.086f, 0.118f, 1.0f};
    Vector4f black = {0.0f, 0.0f, 0.0f, 1.0f};
    Vector4f white = {1.0f, 1.0f, 1.0f, 1.0f};
    Vector4f mask = {1.0f, 1.0f, 0.0f, 1.0f};
}

struct particle {
    DIRECT dir;
    float life;
    float fade;
    Vector2f cPos;
    Vector2f rPos;
    Vector2f vel;
    Vector2f acc;
};

struct dynamicFrame {
    Vector3f ru;
    Vector4f cru;
    Vector3f lu;
    Vector4f clu;
    Vector3f ld;
    Vector4f cld;
    Vector3f rd;
    Vector4f crd;
};

struct texCoordFrame {
    Vector2f ru;
    Vector2f lu;
    Vector2f ld;
    Vector2f rd;
};

struct vertex {
    Vector3f pos;
    Vector3f nor;
    Vector2f tex;
};

void copyVector2f(Vector2f &des, const float *sou) {
    des[0] = sou[0];
    des[1] = sou[1];
}

void loadVector2f(Vector2f &v, float x, float y) {
    v[0] = x;
    v[1] = y;
}

void normalizeVector2f(Vector2f &v) {
    float len = std::sqrt(v[0] * v[0] + v[1] * v[1]);
    if (len == 0)
        return;
    v[0] /= len;
    v[1] /= len;
}

void copyVector3f(Vector3f &des, const float *sou) {
    des[0] = sou[0];
    des[1] = sou[1];
    des[2] = sou[2];
}

void loadVector3f(Vector3f &v, float x, float y, float z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

void crossVector3f(const Vector3f &a, const Vector3f &b, Vector3f &res) {
    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}

void addVector2f(const Vector2f &a, const Vector2f &b, Vector2f &res) {
    res[0] = a[0] + b[0];
    res[1] = a[1] + b[1];
}

void addVector3f(const Vector3f &a, const Vector3f &b, Vector3f &res) {
    res[0] = a[0] + b[0];
    res[1] = a[1] + b[1];
    res[2] = a[2] + b[2];
}

void subVector3f(const Vector3f &a, const Vector3f &b, Vector3f &res) {
    res[0] = a[0] - b[0];
    res[1] = a[1] - b[1];
    res[2] = a[2] - b[2];
}

void normalizeVector3f(Vector3f &v) {
    float len = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (len == 0)
        return;
    v[0] /= len;
    v[1] /= len;
    v[2] /= len;
}

void loadVector4f(Vector4f &v, const float *xyz, float a) {
    v[0] = xyz[0];
    v[1] = xyz[1];
    v[2] = xyz[2];
    v[3] = a;
}

void printVector3f(const Vector3f &v) {
    std::cout << v[0] << " " << v[1] << " " << v[2] << std::endl;
}
