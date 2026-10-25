#ifndef PERSONAGEM_H
#define PERSONAGEM_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "tiro.h"
#include "colidivel.h"

class Personagem : public Colidivel
{
    int vida;
    GLfloat gX;
    GLfloat gY;
    GLfloat width = 30.0;
    GLfloat height = 30.0;
    GLfloat tamanho = 30.0;
    GLfloat theta;
    GLfloat pernas = 40;
    GLfloat velocidade = 5.0;

private:
    void DesenhaPersonagem(GLfloat x, GLfloat y);

public:

    Personagem()
    {
        gX = 0;
        gY = -180; // correção do estado inicial;
        theta = 0;
    };
    void SetTamanho(GLfloat t){
        tamanho =t;
    }
    void SetVida(int v){
        vida = v;
    }
    void setVelocidade(GLfloat v){
        velocidade = v;
    }
    void Desenha()
    {
        DesenhaPersonagem(gX, gY);
    };
    bool Atingido(Colidivel *obj);
    void Girar(int dir);
    void Mover(int dir);
    void MoverLado(int dir);
    Tiro *Atirar();
    virtual void GetColider(GLfloat &x, GLfloat &y, GLfloat &w, GLfloat &h) const override
    {
        x = gX - width;
        y = gY - width;
        w = gX + width;
        h = gY + height;
    };
};

#endif
