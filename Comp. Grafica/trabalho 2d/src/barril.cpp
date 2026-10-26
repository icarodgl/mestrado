#include <math.h>
#include <stdio.h>
#include <vector>

#include "barril.h"
#include "tiro.h"
#include "utils.h"

using namespace std;

void Barril::DesenhaBarril(GLfloat x, GLfloat y)
{
    GLfloat cor[] = {0.24, 0.14, 0.0};
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(cor[0], cor[1], cor[2]);
    glRecti(0, 0, width, height);
    glPopMatrix();
}

void Barril::DesenhaInimigo(GLfloat x, GLfloat y)
{
    vector<GLfloat> CorCorpo = {0.4, 0.156, 0.9};
    vector<GLfloat> CorArma = {0.161, 0.6, 0.45};
    vector<GLfloat> CorPernas = {0.2, 0.0, 0.3};
    GLfloat tamanhoPerna = raioCabeca+(raioCabeca/2);

    controlaPernas(tamanhoPerna, x, y, theta);

    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(theta, 0, 0, 1); // correções para centralizar no boneco

    glColor3f(CorPernas[0], CorPernas[1], CorPernas[2]);
    // perna direita
    glPushMatrix();
    glTranslatef((raioCabeca-15), 0, 0);
    glRecti(0, 0, 10, tamanhoPerna);
    glPopMatrix();

    // perna esquerda
    glPushMatrix();
    glTranslatef(-(raioCabeca - 5), 0, 0);
    glRecti(0, 0, 10, -tamanhoPerna);
    glPopMatrix();

    // corpo
    glColor3f(CorCorpo[0], CorCorpo[1], CorCorpo[2]);
    glPushMatrix();
    DesenhaCirculo(0, 0, raioCabeca);
    glPopMatrix();

    // arma
    glColor3f(CorArma[0], CorArma[1], CorArma[2]);
    glPushMatrix();
    glTranslatef(raioCabeca, 0, 0);
    glRecti(0, 0, 12, 60);
    glPopMatrix();

    glPopMatrix();
}
Tiro *Barril::Atirar(GLfloat deltaTime, GLfloat velocidadeTiro)
{
    if (inimigo and deltaTime > disparo +( 1000.0 / tiroPorSegundo))
    {
        disparo = deltaTime;

        const GLfloat lx = raioCabeca+6; // braco + centro canhao
        const GLfloat ly = 60; // ponta canhao

        const GLfloat DegToRad = M_PI / 180.0f; // Graus pra Radiano
        const GLfloat rad = theta * DegToRad;

        const GLfloat c = cosf(rad);
        const GLfloat s = sinf(rad);

        const GLfloat x = (gX + (width / 2)) + lx * c - ly * s;
        const GLfloat y = (gY + (height / 2)) + lx * s + ly * c;

        Tiro *t = new Tiro(x, y, theta, velocidadeTiro);

        return t;
    }
    return nullptr;
}
