#pragma once
#include <GL/glut.h>

class Colidivel {

public:
    virtual ~Colidivel() = default;

    virtual void GetColider(GLfloat& x, GLfloat& y, GLfloat& w,GLfloat& h) const = 0;
    // utilidade compartilhada — não precisa ser virtual
    bool ColideCom(const Colidivel& obj) const
    {
        
        GLfloat mX,mY,mW,mH;
        GLfloat oX,oY,oW,oH;

        this->GetColider(mX, mY, mW, mH);
        obj.GetColider(oX, oY, oW, oH);

        if((mX <= oW and mY <= oH) and (mW >= oX and mH >= oY ))
            return true;

        return false;
    }
};