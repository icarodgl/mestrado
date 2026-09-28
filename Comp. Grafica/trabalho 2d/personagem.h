#ifndef PERSONAGEM_H
#define	PERSONAGEM_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "barril.h"
#include "tiro.h"
#include "colidivel.h"
class Personagem  : public Colidivel  {
    int vida;
    GLfloat gX; 
    GLfloat gY;
    int gColor;
private:
    void DesenhaPersonagem(GLfloat x, GLfloat y);
public:
    Personagem(){
        gX = 0; 
        gY = -200; 
        gColor = 0;
    };
    void Desenha(){ 
        DesenhaPersonagem(gX, gY);
    };
    bool Atingido(Colidivel* obj);
    void Girar(int dir);
    void Mover(int dir);
    Tiro* Atirar();

    void GetPos(GLfloat& x, GLfloat& y) const override;
    GLfloat GetRaio() const override;
};

#endif

