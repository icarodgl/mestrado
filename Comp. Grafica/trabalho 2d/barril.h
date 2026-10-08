#ifndef BARRIL_H
#define	BARRIL_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "tiro.h"
#include "colidivel.h"

class Barril : public Colidivel  {
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

    void GetPos(GLfloat& x, GLfloat& y) const override;

};

#endif
