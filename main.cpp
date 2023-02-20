#include <GL/glew.h>
#include <GL/freeglut.h>

#include "config.h"
#include "data.h"
#include "shaderLoader.h"
#include "textureLoader.h"
#include "shaderTextBuf.h"
#include "textureTextBuf.h"

unsigned int VAO[2], VBO[3];
unsigned int EBO;
unsigned int simpleShader;
unsigned int particleShader;
unsigned int lightShader;

//着色器中的Uniform变量
int colorLocation;
int samplerLocation;
int stereoSamplerLocation;
int useTextureLocation;
int texColorLocation;
int pointLightColorLocation;
int pointLightPositionLocation;
int eyePositionLocation;

unsigned int particleTexture;
unsigned int pic1;


bool isStereo = false;
bool isDynamic = false;
bool updatable = false;

int width = ORI_WIDTH;
int height = ORI_HEIGHT;

float ratioX = 1.0f;
float ratioY = 1.0f;

void blendFuncForAntiAliasing() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void blendFuncForParticles() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

//生成顶点缓存数据
void genData() {
    genVertices();
    genParticles();

    glGenVertexArrays(2, VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), nullptr);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *)sizeof(Vector3f));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *)(2 * sizeof(Vector3f)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineIndices), lineIndices, GL_STATIC_DRAW);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frames), frames, GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + sizeof(Vector4f), nullptr);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vector4f) + sizeof(Vector3f), (const void *)sizeof(Vector3f));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), nullptr);
}

//导入着色器
void genShader() {
    simpleShader = createShaderProgram(createVertexShader(simple_vert), createFragmentShader(simple_frag));
    particleShader = createShaderProgram(createVertexShader(particle_vert), createFragmentShader(particle_frag));
    lightShader = createShaderProgram(createVertexShader(illumination_vert), createFragmentShader(illumination_frag));

    colorLocation = glGetUniformLocation(simpleShader, "myColor");

    samplerLocation = glGetUniformLocation(particleShader, "gSampler");

    stereoSamplerLocation = glGetUniformLocation(lightShader, "gSampler");
    useTextureLocation = glGetUniformLocation(lightShader, "gUseTexture");
    texColorLocation = glGetUniformLocation(lightShader, "gTexColor");
    pointLightColorLocation = glGetUniformLocation(lightShader, "gPointLight.Color");
    pointLightPositionLocation = glGetUniformLocation(lightShader, "gPointLight.Position");

    eyePositionLocation = glGetUniformLocation(lightShader, "gEyePosition");
}

//导入纹理
void genTexture() {
    particleTexture = createTexture(particle_bmp);
    pic1 = createTexture(pic1_bmp);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, particleTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pic1);
}

//绘制白色五角星
void plainStar() {
    glBindVertexArray(VAO[0]);
    glEnableVertexAttribArray(1);

    glUseProgram(simpleShader);

    glDisable(GL_BLEND);
    glUniform4fv(colorLocation, 1, color::white);
    glDrawArrays(GL_TRIANGLES, 0, 30);

    blendFuncForAntiAliasing();
    glEnable(GL_BLEND);
    glUniform4fv(colorLocation, 1, color::black);
    glLineWidth(2.0);
    glDrawElements(GL_LINE_LOOP, 10, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(1);
}

//绘制立体五角星
void stereoStar() {
    glBindVertexArray(VAO[0]);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glUseProgram(lightShader);

    glActiveTexture(GL_TEXTURE1);
    glUniform1i(stereoSamplerLocation, 1);

    blendFuncForAntiAliasing();
    glEnable(GL_BLEND);
    glUniform4fv(texColorLocation, 1, color::sjtuRed);
    glUniform3fv(pointLightColorLocation, 1, color::white);
    glUniform3f(pointLightPositionLocation, -0.3f, 0.7f, 1.0f);
    glUniform3f(eyePositionLocation, 0.0f, 0.0f, 1.0f);

    //先绘制一层作为底色，防止边缘割裂
    glUniform1i(useTextureLocation, 1);
    glDrawArrays(GL_TRIANGLES, 0, 30);

    glUniform1i(useTextureLocation, 0);
    glDrawArrays(GL_TRIANGLES, 0, 30);

    // 绘制包含贴图的三角形
    glUniform1i(useTextureLocation, 1);
    glDrawArrays(GL_TRIANGLES, 6, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

//绘制动态背景
void dynamicBackground() {
    glBindVertexArray(VAO[1]);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glUseProgram(particleShader);

    //更新粒子属性
    updateParticles(ratioX, ratioY, (particle *)particles, 5 * MAX_PARTICLES);
    updateParticles(ratioX, ratioY, particles[5], MAX_PARTICLES, updatable);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frames), frames, GL_STREAM_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(samplerLocation, 0);

    blendFuncForParticles();
    glEnable(GL_BLEND);
    glDrawArrays(GL_QUADS, 0, 24 * MAX_PARTICLES);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

void display() {
    if (isStereo)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    else
        glClearColor(0.6f, 0.85f, 0.92f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (isStereo && isDynamic)
        dynamicBackground();
    if (isStereo)
        stereoStar();
    else
        plainStar();

    glutSwapBuffers();
}

void resize(int w, int h) {
    width = w;
    height = h;
    if(w < 100) w = 100;
    if(h < 100) h = 100;

    glViewport(0, 0, w, h);
    if (w > h) {
        ratioX = (float)h / (float)w;
        ratioY = 1.0f;
    } else {
        ratioX = 1.0f;
        ratioY = (float)w / (float)h;
    }

    resizeVertices(ratioX, ratioY);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glutPostRedisplay();
}

void update(int) {
    glutPostRedisplay();
    glutTimerFunc(16, update, 1);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'c':
            isStereo = !isStereo;
            if (!isStereo && isDynamic) {
                isDynamic = false;
                genParticles();
            }
            break;
        case 's':
            if (!isStereo)
                break;
            isDynamic = !isDynamic;
            if (isDynamic)
                genParticles();
            break;
        default:
            break;
    }
}

void mouse(int button, int state, int x, int y) {
    if (button != 0) //鼠标左键点击
        return;
    if (state == 0)
        updatable = true;
    else {
        updatable = false;
        return;
    }

    //根据屏幕长宽比例计算坐标
    float xp, yp;
    int size = width < height ? width : height;
    xp = ((GLfloat)x - (GLfloat)width * 0.5f) / (GLfloat)size * 2.0f;
    yp = ((GLfloat)height * 0.5f - (GLfloat)y) / (GLfloat)size * 2.0f;

    //将粒子重置位置设置到鼠标点击位置
    updateCentral((float)xp, (float)yp, particles[5]);
    auto *p = (particle *)particles + 5 * MAX_PARTICLES;
    for (int i = 0; i < MAX_PARTICLES; i++, p++)
        if (p->life < 0.0f)
            genParticle(xp, yp, p, 5.0f);
}

void motion(int x, int y) {
    //同上
    float xp, yp;
    int size = width < height ? width : height;
    xp = ((GLfloat)x - (GLfloat)width * 0.5f) / (GLfloat)size * 2.0f;
    yp = ((GLfloat)height * 0.5f - (GLfloat)y) / (GLfloat)size * 2.0f;
    updateCentral((float)xp, (float)yp, particles[5]);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(ORI_WIDTH, ORI_HEIGHT);
    glutCreateWindow("star");

    glewInit();

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutTimerFunc(16, update, 1);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    genData();
    genShader();
    genTexture();

    glutMainLoop();

    glDeleteProgram(simpleShader);
    glDeleteProgram(particleShader);

    return 0;
}
