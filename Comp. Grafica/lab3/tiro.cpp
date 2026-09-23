#include "tiro.h"
#include <math.h>
#define DISTANCIA_MAX 500

void Tiro::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    int segmentos = 8;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0); // centro
        for (int i = 0; i <= segmentos; i++) {
            float angulo = 2.0f * M_PI * i / segmentos;
            float x = 0 + radius * cosf(angulo);
            float y = 0 + radius * sinf(angulo);
            glVertex2f(x, y);
        }
    glEnd();
}

void Tiro::DesenhaTiro(GLfloat x, GLfloat y)
{
    glPushMatrix();
        glTranslatef(x,y, 0);
        DesenhaCirc(radiusTiro,1,1,1);
    glPopMatrix();

}

void Tiro::Move(GLfloat delta) 
{   
    
    GLfloat rad = this->gDirectionAng * M_PI / 180.0f;
    GLfloat vx = -sinf(rad) * this->gVel * delta;
    GLfloat vy = cosf(rad) * this->gVel * delta;

    this->gX += vx;
    this->gY += vy;
}

bool Tiro::Valido()
{
    if (gX > DISTANCIA_MAX || gY > DISTANCIA_MAX)
        return false;
    return true;
}
