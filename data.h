/********************************************
 * 此文件定义了所有关于顶点数据生成、计算与更新的方法
 *******************************************/

#pragma once

#include <iostream>
#include <cmath>

#include "config.h"

float largeRadius = 0.7854;
float middleRadius = 0.32f;
float smallRadius = 0.3;

vertex verticesRaw[30];        //保存在内存中的五角星顶点数据
vertex vertices[30];           //传入VBO的五角星顶点缓存
unsigned int lineIndices[10];  //绘制五角星黑色外边框的索引

particle particles[6][MAX_PARTICLES];       //背景粒子属性数据
dynamicFrame frames[6 * MAX_PARTICLES];     //背景粒子坐标缓存
texCoordFrame texCoords[6 * MAX_PARTICLES]; //背景粒子纹理坐标缓存

void genVertices() {
    for (int i = 0; i < 10; i++)
        lineIndices[i] = i * 3;

    //顶点坐标
    float angle, radius;
    for (int i = 0; i < 30; i += 3) {
        angle = (float)((18.0 + 12.0 * i) / 180.0 * PI);
        radius = i % 2 ? smallRadius : largeRadius;
        loadVector3f(verticesRaw[i].pos, radius * std::cos(angle), radius * std::sin(angle), 0.0f);
    }
    for (int i = 0; i < 30; i += 3) {
        copyVector3f(verticesRaw[i + 1].pos, verticesRaw[(i + 3) % 30].pos);
        loadVector3f(verticesRaw[i + 2].pos, 0.0f, 0.0f, 0.08f);
    }

    //计算每个顶点的法向量
    Vector3f v1, v2, normal;
    for (int i = 0; i < 30; i += 3) {
        subVector3f(verticesRaw[i].pos, verticesRaw[i + 2].pos, v1);
        subVector3f(verticesRaw[i + 1].pos, verticesRaw[i + 2].pos, v2);
        crossVector3f(v1, v2, normal);
        normalizeVector3f(normal);
        copyVector3f(verticesRaw[i].nor, normal);
        copyVector3f(verticesRaw[i + 1].nor, normal);
        copyVector3f(verticesRaw[i + 2].nor, normal);
    }

    //第3个三角形的纹理坐标
    loadVector2f(verticesRaw[6].tex, 1.0f, 1.0f);
    loadVector2f(verticesRaw[7].tex, 0.618f, 0.618f);
    loadVector2f(verticesRaw[8].tex, 1.0f, 0.447f);

    memcpy((char *)vertices, (char *)verticesRaw, sizeof(verticesRaw));
}

void resizeVertices(float ratioX, float ratioY) {
    for (int i = 0; i < 30; i++) {
        vertices[i].pos[0] = verticesRaw[i].pos[0] * ratioX;
        vertices[i].pos[1] = verticesRaw[i].pos[1] * ratioY;
    }
}

void genParticle(float x, float y, particle *particle, float rate) {
    particle->life = 1.0f;
    particle->fade = float(rand() % 97 + 3) / 1000.0f;
    loadVector2f(particle->cPos, x, y);
    loadVector2f(particle->rPos, x, y);
    loadVector2f(particle->acc, float(rand() % 40 - 20) / 8000000.0f, float(rand() % 40 - 20) / 8000000.0f);

    float vel = float(rand() % 100) / 30000.0f * rate;
    if (particle->dir == D_FIXED) { //第一种速度设置方式：由屏幕中心指向四周
        Vector2f direction = {x, y};
        normalizeVector2f(direction);
        loadVector2f(particle->vel, vel * direction[0], vel * direction[1]);
    } else {  //第二种速度设置方式：由当前位置随机向四周发散
        float angle = PI * (float)(rand() % 100) / 50.0f;
        loadVector2f(particle->vel, vel * std::cos(angle), vel * std::sin(angle));
    }
}

void genParticles() {
    srand(time(nullptr));
    float angle;
    auto *p = (particle *)particles;
    for (int i = 0; i < 5 * MAX_PARTICLES; i++, p++) {
        p->dir = D_FIXED;
        angle = (float)((0.06 * i) / 180.0 * PI);
        genParticle(middleRadius * std::cos(angle), middleRadius * std::sin(angle), p, 5.0f);
    }
    for (int i = 0; i < MAX_PARTICLES; i++, p++) {
        p->dir = D_RAND;
        genParticle(0.0f, 0.0f, p, 5.0f);
    }

    //每个顶点的纹理坐标
    for (auto & texCoord : texCoords) {
        loadVector2f(texCoord.ru, 1, 1);
        loadVector2f(texCoord.lu, 0, 1);
        loadVector2f(texCoord.ld, 0, 0);
        loadVector2f(texCoord.rd, 1, 0);
    }
}

//更新粒子属性
void updateParticles(float ratioX, float ratioY, particle *p, int num, bool updatable=true) {
    int offset = (int)(p - (particle *)particles);
    float x, y, mask;
    Vector3f color;
    for (int i = 0; i < num; i++, offset++) {
        if (p[i].life < 0.0f)   //如果生命值为0则不继续更新
            continue;
        p[i].life -= p[i].fade; //减少生命值

        x = p[i].cPos[0];
        y = p[i].cPos[1];
        //设置顶点位置
        loadVector3f(frames[offset].ru, (x + 0.04f) * ratioX, (y + 0.04f) * ratioY, 0.0f);
        loadVector3f(frames[offset].lu, (x - 0.04f) * ratioX, (y + 0.04f) * ratioY, 0.0f);
        loadVector3f(frames[offset].ld, (x - 0.04f) * ratioX, (y - 0.04f) * ratioY, 0.0f);
        loadVector3f(frames[offset].rd, (x + 0.04f) * ratioX, (y - 0.04f) * ratioY, 0.0f);

        copyVector3f(color, color::mask);
        mask = (float)((clock()) / 3000.0 * PI);
        color[0] *= (std::sin(mask) * 0.4f + 0.6f);
        color[1] *= (std::cos(mask) * 0.4f + 0.6f);
        //将粒子生命值直接作为颜色的alpha值，当生命值为0时，粒子消失
        loadVector4f(frames[offset].cru, color, p[i].life);
        loadVector4f(frames[offset].clu, color, p[i].life);
        loadVector4f(frames[offset].cld, color, p[i].life);
        loadVector4f(frames[offset].crd, color, p[i].life);

        //更新位置
        addVector2f(p[i].vel, p[i].cPos, p[i].cPos);
        addVector2f(p[i].acc, p[i].vel, p[i].vel);

        if (p[i].life < 0.0f && updatable)
            genParticle(p[i].rPos[0], p[i].rPos[1], p + i, 1.0f);
    }
}

void updateCentral(float x, float y, particle *p) {
    for (int i = 0; i < MAX_PARTICLES; i++)
        loadVector2f(p[i].rPos, x, y);
}