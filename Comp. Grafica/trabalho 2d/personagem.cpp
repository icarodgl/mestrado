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
    vector<GLfloat> CorPernas = {0.46, 0.92, 0.009};
    vector<GLfloat> CorArma = {0.161, 0.6, 0.45};

    // corpo
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glColor3f(CorCorpo[0], CorCorpo[1], CorCorpo[2]);
    DesenhaCirculo(x, y);
    glPopMatrix();

    //perna dir
    glColor3f(CorPernas[0], CorPernas[1], CorPernas[2]);

    glPushMatrix();
    glTranslatef(x+20, y, 0.0f);
    glRecti(0,0,10,50);
    glPopMatrix();
    // // perna esq
    glPushMatrix();
    glTranslatef(x-20, y, 0.0f);
    glRecti(0,0,10,50);
    glPopMatrix();
}
void Personagem::Girar(int dir)
{
    gX += dir;
}

void Personagem::Mover(int dir)
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
