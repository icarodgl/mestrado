#include <math.h>
#include <stdio.h>
#include <vector>
#include "barril.h"
#include "personagem.h"
#include "tiro.h"
using namespace std;

void DesenhaCirculo(GLfloat x, GLfloat y)
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

void Personagem::DesenhaPersonagem(GLfloat x, GLfloat y)
{

    vector<GLfloat> CorCorpo = {0.75, 0.156, 0.09};
    vector<GLfloat> CorPernas = {0.9, 0.9, 0.9};
    vector<GLfloat> CorArma = {0.161, 0.6, 0.45};

    glColor3f(CorPernas[0], CorPernas[1], CorPernas[2]);


    glPushMatrix();
        glRotatef(theta, 0, 0, 1);

    // perna dir

    glPushMatrix();
    glTranslatef(x + 14, y, 0.0f);
    glRecti(0, 0, 10, 50);
    glPopMatrix();
    // // perna esq
    glPushMatrix();
    glTranslatef(x - 26, y - 50, 0.0f);
    glRecti(0, 0, 10, 50);
    glPopMatrix();

    // corpo
    glColor3f(CorCorpo[0], CorCorpo[1], CorCorpo[2]);

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    DesenhaCirculo(x, y);
    glPopMatrix();

    // arma

    glColor3f(CorArma[0], CorArma[1], CorArma[2]);
    glPushMatrix();
    glTranslatef(x + 30, y, 0.0f);
    glRecti(0, 0, 12, 60);
    glPopMatrix();

    glPopMatrix();
}
void Personagem::Girar(int dir)
{
    if ((theta < 45 and dir > 0) or ( dir < 0 and theta > -45))
        theta += dir;
}

void Personagem::Mover(int dir)
{
    GLfloat rad = this->theta * M_PI / 180.0f;
    GLfloat vx = -sinf(rad) * dir*5;
    GLfloat vy = cosf(rad) * dir*5;

    this->gX += vx;
    this->gY += vy;
    // gX += dir;
}
void Personagem::MoverLado(int dir)
{
    gY += dir;
}
Tiro *Personagem::Atirar()
{
    Tiro *t = new Tiro(0.0, 0.0, 0.0);
    return t;
}

bool Personagem::Atingido(Colidivel *obj)
{
    if (!obj)
        return false;
    return ColideCom(*obj);
}

GLfloat Personagem::GetRaio() const
{
}
