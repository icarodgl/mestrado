#include "alvo.h"
#include <math.h>
#include <stdio.h>

void Alvo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    int seg = 32;
    glColor3f(R,G,B);
            
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0); // centro
        for (int i = 0; i <= seg; i++) {

            float angulo = 2.0f * M_PI * i / seg;
            float x = 0 + radius * cosf(angulo);
            float y = 0 + radius * sinf(angulo);
            glVertex2f(x, y);
        }
    glEnd();
}

void Alvo::DesenhaAlvo(GLfloat x, GLfloat y)
{
    glPushMatrix();
        glTranslatef(x, y, 0);
        DesenhaCirc(radiusAlvo,1,1,1);
    glPopMatrix();
}

void Alvo::Recria(GLfloat x, GLfloat y)
{
    this->gX =x;
    this->gY = y;
}

bool Alvo::Atingido(Tiro *tiro)
{   
    GLfloat xT,yT;
    tiro->GetPos(xT,yT);

    GLfloat x0 = this->gX-radiusAlvo;
    GLfloat y0 = this->gY-radiusAlvo;
    GLfloat xa = this->gX+radiusAlvo;
    GLfloat ya = this->gY+radiusAlvo;

    if((xT >= x0 && xT <= xa ) && (yT >= y0 && yT <= ya)){
        return true;
    }
    return false;
}
