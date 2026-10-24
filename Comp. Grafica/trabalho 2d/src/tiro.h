#ifndef TIRO_H
#define TIRO_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "colidivel.h"

#define radiusTiro 5

class Tiro : public Colidivel
{
    GLfloat height = 10;
    GLfloat width = 10;
    GLfloat gX;
    GLfloat gY;
    GLfloat gDirectionAng;
    GLfloat gVel;

private:
    void DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B);
    void DesenhaTiro(GLfloat x, GLfloat y);

public:
    Tiro(GLfloat x, GLfloat y, GLfloat directionAng)
    {
        gX = x;
        gY = y;
        gDirectionAng = directionAng;
        gVel = 0.1;
    };
    void Desenha()
    {
        DesenhaTiro(gX, gY);
    };
    void Move(GLfloat deltaTime);
    
    bool Valido();

    virtual void GetColider(GLfloat& x, GLfloat& y, GLfloat& w,GLfloat& h) const override {
        x = gX - (width/4);
        y = gY - (height/4);
        w = gX + (width/4);
        h = gY + (height/4);
    };
};
#endif /* TIRO_H */
