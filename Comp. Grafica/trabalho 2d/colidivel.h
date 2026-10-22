#pragma once
#include <GL/glut.h>

class Colidivel {
public:
    virtual ~Colidivel() = default;

    virtual void GetPos(GLfloat& x, GLfloat& y) const = 0;
    virtual void GetTamanho(GLfloat& x, GLfloat& y) const = 0;

    // utilidade compartilhada — não precisa ser virtual
    bool ColideCom(const Colidivel& obj) const
    {
        
        // GLfloat mX,mY,mXl,mYl = 0.0;
        // GLfloat oX,oY,oXl,oYl = 0.0;

        // this->GetPos(mX, mY);
        // this->GetTamanho(mXl,mYl);
        // obj.GetPos(oX, oY);
        // obj.GetTamanho(oXl,oYl);

        // if((mX <= oXl and mY <= oYl) and (mXl >= oX and mYl >= oY ))
        //     return true;

        return false;
    }

    // bool checkSquareCollision(float x0_a, float y0_a, float x1_a, float y1_a,
    //                       float x0_b, float y0_b, float x1_b, float y1_b) {
    // // Calcula os limites (min e max) do primeiro quadrado nos eixos X e Y
    // float minX_a = std::min(x0_a, x1_a);
    // float maxX_a = std::max(x0_a, x1_a);
    // float minY_a = std::min(y0_a, y1_a);
    // float maxY_a = std::max(y0_a, y1_a);

    // // Verifica se há sobreposição no eixo X
    // bool overlapX = (std::max(minX_a, minX_b) < std::min(maxX_a, maxX_b));

    // // Verifica se há sobreposição no eixo Y
    // bool overlapY = (std::max(minY_a, minY_b) < std::min(maxY_a, maxY_b));

    // // Os quadrados se sobreposem apenas se houver sobreposição nos dois eixos
    // return overlapX && overlapY;
};