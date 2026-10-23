#ifndef BARRIL_H
#define	BARRIL_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include "tiro.h"
#include "colidivel.h"

#define COL_X 10.0
#define COL_Y 10.0
#define DISTANCIA_MAX 400


class Barril : public Colidivel  {
    int vida;
    GLfloat Height = 80.0;
    GLfloat Width = 160.0;
    GLfloat gX; 
    GLfloat gY;
    GLfloat gVel = 1.0;
private:
    void DesenhaBarril(GLfloat x, GLfloat y);
    void DesenhaTexto(float x, float y, const std::string& texto, void* fonte) const
    {
        glRasterPos2f(x, y - Height);
        for (char c : texto)
            glutBitmapCharacter(fonte, c);
    };

public:
    Barril(GLfloat x, GLfloat y, int vida){
        gX = x-(Width/2); // correção da metade do tamanho do barril
        gY = y;
        this->vida = vida;
    };
    void Desenha(){ 
        DesenhaBarril(gX, gY);
        DesenhaVida();
    };
    void Atingido(){
        vida -= 1;
    };

    void Move(GLfloat deltaTime){
        gY -= 0.8 + gVel * deltaTime;
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
    void DesenhaVida() const
{
    std::string txt = std::to_string(vida);
    glColor3f(1.0f, 1.0f, 1.0f);
    DesenhaTexto(gX, gY + Height, txt, GLUT_BITMAP_HELVETICA_18);
}
};

#endif



