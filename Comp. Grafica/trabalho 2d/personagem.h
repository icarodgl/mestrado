#ifndef PERSONAGEM_H
#define PERSONAGEM_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "barril.h"
#include "tiro.h"
#include "colidivel.h"

class Personagem : public Colidivel
{
    int vida;
    GLfloat gX;
    GLfloat gY;

private:
    void DesenhaPersonagem(GLfloat x, GLfloat y);
    GLfloat GetTamanho() const override
    {
        return 10.0;
    }

public:
    Personagem()
    {
        gX = 0;
        gY = 0;
    };
    void Desenha()
    {
        DesenhaPersonagem(gX, gY);
    };
    bool Atingido(Colidivel *obj);
    void Girar(int dir);
    void Mover(int dir);
    Tiro *Atirar();
    void GetPos(GLfloat &xOut, GLfloat &yOut) const override
    {
        xOut = gX;
        yOut = gY;
    }
    GLfloat GetRaio() const;
};

#endif
