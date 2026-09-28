#ifndef PERSONAGEM_H
#define	PERSONAGEM_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "barril.h"
#include "tiro.h"

class Personagem {
    int vida;
    GLfloat gX; 
    GLfloat gY;
    int gColor;
private:
    void DesenhaPersonagem(GLfloat x, GLfloat y);
public:
    Personagem(GLfloat x, GLfloat y){
        gX = x; 
        gY = y;
        gColor = 0;
    };
    void Desenha(){ 
        DesenhaPersonagem(gX, gY);
    };
    bool Atingido(Barril *tiro);
    void Girar(int dir);
    void Mover(int dir);
    Tiro* Atirar();
};

#endif

