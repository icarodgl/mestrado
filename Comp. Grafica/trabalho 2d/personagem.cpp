#include <math.h>
#include <stdio.h>
#include "barril.h"
#include "personagem.h"

    void Personagem::Girar(int dir){

    }

    void Personagem::Mover(int dir){

    }

    Tiro* Personagem::Atirar(){}

    bool Personagem::Atingido(Colidivel* obj){

        if (!obj) return false;
        return ColideCom(*obj);

    }
    void Personagem::GetPos(GLfloat& x, GLfloat& y)const{
        x = this->gX;
        y = this->gY;
    }

    GLfloat Personagem::GetRaio()const{

    }