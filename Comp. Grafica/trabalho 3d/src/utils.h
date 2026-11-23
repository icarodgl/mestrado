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
// ---------- helpers de cone ----------
static void normalize3(float &x, float &y, float &z)
{
    float n = sqrtf(x*x + y*y + z*z);
    if (n < 1e-8f) { x = 0; y = 1; z = 0; return; }
    x /= n; y /= n; z /= n;
}

// Constrói base ortonormal (U,V) perpendicular a D
static void basePerpendicular(float Dx, float Dy, float Dz,
                              float &Ux, float &Uy, float &Uz,
                              float &Vx, float &Vy, float &Vz)
{
    // escolhe um "up" que NÃO seja paralelo a D
    float ax, ay, az;
    if (fabsf(Dz) < 0.9f) { ax = 0; ay = 0; az = 1; }
    else                  { ax = 1; ay = 0; az = 0; }

    // U = normalize(up × D)
    Ux = ay*Dz - az*Dy;
    Uy = az*Dx - ax*Dz;
    Uz = ax*Dy - ay*Dx;
    normalize3(Ux, Uy, Uz);

    // V = D × U
    Vx = Dy*Uz - Dz*Uy;
    Vy = Dz*Ux - Dx*Uz;
    Vz = Dx*Uy - Dy*Ux;
    normalize3(Vx, Vy, Vz);
}

// Gera direção dentro de um cone de semiângulo halfAngleGraus em torno de D
static void direcaoNoCone(float Dx, float Dy, float Dz,
                          float halfAngleGraus,
                          float &ox, float &oy, float &oz)
{
    normalize3(Dx, Dy, Dz);
    float Ux, Uy, Uz, Vx, Vy, Vz;
    basePerpendicular(Dx, Dy, Dz, Ux, Uy, Uz, Vx, Vy, Vz);

    // cos(phi) uniforme na calota esférica -> densidade uniforme no cone
    float halfRad = halfAngleGraus * (float)M_PI / 180.0f;
    float cosMin  = cosf(halfRad);
    float cosPhi  = cosMin + (1.0f - cosMin) * ((float)rand() / RAND_MAX);
    float sinPhi  = sqrtf(fmaxf(0.0f, 1.0f - cosPhi * cosPhi));

    float theta = ((float)rand() / RAND_MAX) * 2.0f * (float)M_PI;
    float cs = cosf(theta), sn = sinf(theta);

    // direção final = cosPhi*D + sinPhi*(cs*U + sn*V)
    ox = cosPhi * Dx + sinPhi * (cs * Ux + sn * Vx);
    oy = cosPhi * Dy + sinPhi * (cs * Uy + sn * Vy);
    oz = cosPhi * Dz + sinPhi * (cs * Uz + sn * Vz);
    normalize3(ox, oy, oz);
}
// ---------- helpers para desenhar o cone no debug ----------
static void norm3(float &x, float &y, float &z)
{
    float n = sqrtf(x*x + y*y + z*z);
    if (n < 1e-8f) { x = 0; y = 1; z = 0; return; }
    x /= n; y /= n; z /= n;
}

static void basePerp(float Dx, float Dy, float Dz,
                     float &Ux, float &Uy, float &Uz,
                     float &Vx, float &Vy, float &Vz)
{
    float ax, ay, az;
    if (fabsf(Dz) < 0.9f) { ax = 0; ay = 0; az = 1; }
    else                  { ax = 1; ay = 0; az = 0; }

    Ux = ay*Dz - az*Dy;
    Uy = az*Dx - ax*Dz;
    Uz = ax*Dy - ay*Dx;
    norm3(Ux, Uy, Uz);

    Vx = Dy*Uz - Dz*Uy;
    Vy = Dz*Ux - Dx*Uz;
    Vz = Dx*Uy - Dy*Ux;
    norm3(Vx, Vy, Vz);
}

#endif