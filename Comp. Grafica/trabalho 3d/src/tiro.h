#ifndef TIRO_H
#define TIRO_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "colidivel.h"
#include "configuracao.h"
#include "objloader.h"

#define radiusTiro 5 // Ainda pode ser usado para a colisão

class Tiro : public Colidivel
{
    GLfloat height = 10;
    GLfloat width = 10;
    GLfloat alturaMax = 500;
    GLfloat larguraMax = 500;
    GLfloat gX;
    GLfloat gY;
    GLfloat gDirectionAng;
    GLfloat gVel;

    // Variáveis estáticas para carregar o .obj apenas uma vez
    static mesh  malhaBala;
    static bool  malhaCarregada;
    static float escalaMalha;
    static float minYMalha;
    
    void garanteMalha();

public:
    Tiro(GLfloat x, GLfloat y, GLfloat directionAng, GLfloat velocidade)
    {
        gX = x;
        gY = y;
        gDirectionAng = directionAng;
        gVel = velocidade;
        garanteMalha(); // Garante que a malha seja carregada
    };
    
    void Desenha();
    void Move(GLfloat deltaTime);
    bool Valido();

    virtual void GetColider(GLfloat& x, GLfloat& y, GLfloat& w, GLfloat& h) const override {
        x = gX - (width/4);
        y = gY - (height/4);
        w = gX + (width/4);
        h = gY + (height/4);
    };

    void Configurar(Configuracao conf) {
        alturaMax = conf.arena.altura;
        larguraMax = conf.arena.largura;
    }
};
#endif /* TIRO_H */