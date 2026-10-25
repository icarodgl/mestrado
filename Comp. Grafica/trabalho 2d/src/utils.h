#pragma once
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
inline void DesenhaCirculo(GLfloat x, GLfloat y)
{
    int radius = 30;
    int segmentos = 32;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0); // centro
    for (int i = 0; i <= segmentos; i++)
    {
        float angulo = 2.0f * M_PI * i / segmentos;
        float x = 0 + radius * cosf(angulo);
        float y = 0 + radius * sinf(angulo);
        glVertex2f(x, y);
    }
    glEnd();
}

inline void controlaPernas(GLfloat &tamanhoPerna, GLfloat x, GLfloat y, GLfloat theta){
    tamanhoPerna = 50.0 * sin((y+x+theta)/14);
}