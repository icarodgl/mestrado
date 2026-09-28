#ifndef BARRIL_H
#define	BARRIL_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "tiro.h"

class Barril {
    int vida;
    GLfloat gX; 
    GLfloat gY;
    int gColor;
private:
    void DesenhaBarril(GLfloat x, GLfloat y);
public:
    Barril(GLfloat x, GLfloat y){
        gX = x; 
        gY = y;
        gColor = 0;
    };
    void Desenha(){ 
        DesenhaBarril(gX, gY);
    };
    bool Atingido(Tiro *tiro);

    void Move(GLfloat deltaTime){}

    bool Valido();
};

#endif
