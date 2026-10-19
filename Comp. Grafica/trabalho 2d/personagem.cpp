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

   
    glPushMatrix();
        glTranslatef(x, y, 0);
        glRotatef(theta, 0, 0, 1); // correções para centralizar no boneco
    
        // perna direita
        glColor3f(CorPernas[0], CorPernas[1], CorPernas[2]);
        glPushMatrix();
            glTranslatef(14, 0, 0);
            glRecti(0, 0, 10, 50);
        glPopMatrix();

        // perna esquerda
        glPushMatrix();
            glTranslatef(-26, -50, 0);
            glRecti(0, 0, 10, 50);
        glPopMatrix();

        // corpo
        glColor3f(CorCorpo[0], CorCorpo[1], CorCorpo[2]);
        glPushMatrix();
            DesenhaCirculo(0, 0);
        glPopMatrix();

        // arma
        glColor3f(CorArma[0], CorArma[1], CorArma[2]);
        glPushMatrix();
            glTranslatef(30, 0, 0);
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
    GLfloat vx = -sinf(rad) * dir;
    GLfloat vy = cosf(rad) * dir;

    this->gX += vx;
    this->gY += vy;
}
void Personagem::MoverLado(int dir)
{
    gX += dir;
}
Tiro *Personagem::Atirar()
{
    const GLfloat lx = 36; // braco + centro canhao
    const GLfloat ly = 60; // ponta canhao

    const GLfloat DegToRad = M_PI / 180.0f; // Graus pra Radiano
    const GLfloat rad = theta * DegToRad;

    const GLfloat c = cosf(rad);
    const GLfloat s = sinf(rad);

    const GLfloat x = gX + lx * c - ly * s;
    const GLfloat y = gY + lx * s + ly * c;

    Tiro *t = new Tiro(x, y, theta);
    
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