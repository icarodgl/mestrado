#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <random>

inline int IntAleatorio(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

inline float FloatAleatorio(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}
inline void DesenhaCirculo(GLfloat x, GLfloat y, GLfloat raio)
{
    int segmentos = 32;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0); // centro
    for (int i = 0; i <= segmentos; i++)
    {
        float angulo = 2.0f * M_PI * i / segmentos;
        x = raio * cosf(angulo);
        y = raio * sinf(angulo);
        glVertex2f(x, y);
    }
    glEnd();
}

typedef float mat4[16];

inline void mat4_identity(mat4 m) {
    for (int i = 0; i < 16; ++i) m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
}

inline void mat4_mul(const mat4 a, const mat4 b, mat4 &out) {

    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            out[col*4 + row] =
                a[0*4 + row] * b[col*4 + 0] +
                a[1*4 + row] * b[col*4 + 1] +
                a[2*4 + row] * b[col*4 + 2] +
                a[3*4 + row] * b[col*4 + 3];
        }
    }
}

inline void mat4_translate(mat4 m, float x, float y, float z) {
    mat4_identity(m);
    m[12] = x; m[13] = y; m[14] = z;
}

inline void mat4_scale(mat4 m, float s) {
    mat4_identity(m);
    m[0] = s; m[5] = s; m[10] = s;
}

inline void mat4_rotateX(mat4 m, float deg) {
    float r = deg * (float)M_PI / 180.0f;
    float c = cosf(r), s = sinf(r);
    mat4_identity(m);
    m[5]  = c;  m[6]  = s;
    m[9]  = -s; m[10] = c;
}

inline void mat4_rotateY(mat4 m, float deg) {
    float r = deg * (float)M_PI / 180.0f;
    float c = cosf(r), s = sinf(r);
    mat4_identity(m);
    m[0] = c;  m[2] = -s;
    m[8] = s;  m[10] = c;
}

inline void mat4_rotateZ(mat4 m, float deg) {
    float r = deg * (float)M_PI / 180.0f;
    float c = cosf(r), s = sinf(r);
    mat4_identity(m);
    m[0] = c;  m[1] = s;
    m[4] = -s; m[5] = c;
}

inline void mat4_transformPoint(const mat4 m,
                                float x, float y, float z,
                                float &ox, float &oy, float &oz) {
    ox = m[0]*x + m[4]*y + m[8]*z  + m[12];
    oy = m[1]*x + m[5]*y + m[9]*z  + m[13];
    oz = m[2]*x + m[6]*y + m[10]*z + m[14];
}

#endif