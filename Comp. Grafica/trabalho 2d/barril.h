#ifndef BARRIL_H
#define	BARRIL_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "tiro.h"
#include "colidivel.h"

#define COL_X 10.0
#define COL_Y 10.0
#define DISTANCIA_MAX 500


class Barril : public Colidivel  {
    int vida;
    GLfloat gX; 
    GLfloat gY;
    GLfloat gVel = 1.1;
private:
    void DesenhaBarril(GLfloat x, GLfloat y);
public:
    Barril(GLfloat x, GLfloat y){
        gX = x; 
        gY = y;
    };
    void Desenha(){ 
        DesenhaBarril(gX, gY);
    };
    bool Atingido(Tiro *tiro);

    void Move(GLfloat deltaTime){
        gY += gVel * deltaTime;
    }                                           

    bool Valido(){
    if(vida <= 0) return false;

    if (gX > DISTANCIA_MAX || gY > DISTANCIA_MAX)
        return false;

    return true;
}
               
    void GetPos(GLfloat &xOut, GLfloat &yOut) const override
    {
        xOut = gX;
        yOut = gY;
    };
    void GetTamanho(GLfloat& x, GLfloat& y) const override
    {
        x = COL_X;
        y = COL_Y;

    }
};

#endif
