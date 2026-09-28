#pragma once
#include <GL/glut.h>

class Colidivel {
public:
    virtual ~Colidivel() = default;

    virtual void GetPos(GLfloat& x, GLfloat& y) const = 0;
    virtual GLfloat GetRaio() const = 0;

    // utilidade compartilhada — não precisa ser virtual
    bool ColideCom(const Colidivel& outro) const
    {
        GLfloat x1, y1, x2, y2;
        GetPos(x1, y1);
        outro.GetPos(x2, y2);

        const GLfloat dx = x1 - x2;
        const GLfloat dy = y1 - y2;
        const GLfloat r  = GetRaio() + outro.GetRaio();
        return dx*dx + dy*dy <= r*r;
    }
};