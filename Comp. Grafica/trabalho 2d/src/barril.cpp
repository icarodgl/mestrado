#include <math.h>
#include <stdio.h>
#include <vector>

#include "barril.h"
#include "tiro.h"


using namespace std;


void Barril::DesenhaBarril(GLfloat x, GLfloat y){
    GLfloat cor[] =  {0.24, 0.14, 0.0};
    int h = 20;
    int w = 40;
    glPushMatrix();
        glTranslatef(x, y, 0);
        glColor3f(cor[0], cor[1], cor[2]);
        glRecti(0,0,160,80);
    glPopMatrix();
}
