#ifndef PERSONAGEM_H
#define PERSONAGEM_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "barril.h"
#include "tiro.h"
#include "colidivel.h"

#define COL_X 10.0;
#define COL_Y 10.0;

class Personagem : public Colidivel
{
    int vida;
    GLfloat gX;
    GLfloat gY;
    GLfloat theta;

private:
    void DesenhaPersonagem(GLfloat x, GLfloat y);
    void GetTamanho(GLfloat& x, GLfloat& y) const override
    {
        x = gX + COL_X;
        y = gY + COL_Y;

    }

public:
    Personagem()
    {
        gX = 0;
        gY = 0;
        theta = 0;
    };
    void Desenha()
    {
        DesenhaPersonagem(gX, gY);
    };
    bool Atingido(Colidivel *obj);
    void Girar(int dir);
    void Mover(int dir);
    void MoverLado(int dir);
    Tiro *Atirar();
    void GetPos(GLfloat &xOut, GLfloat &yOut) const override
    {
        xOut = gX;
        yOut = gY;
    }
    GLfloat GetRaio() const;
};

#endif
