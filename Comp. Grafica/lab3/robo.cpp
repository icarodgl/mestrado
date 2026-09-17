#include "robo.h"
#include <math.h>

void Robo::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R,G,B);
    // glBegin(GL_LINE_LOOP);
    //     glVertex2i(0,        -width/2);
    //     glVertex2i(height, -width/2);
    //     glVertex2i(height,  width/2);
    //     glVertex2i(0,         width/2);
    // glEnd();

    glRecti(-width,-height,width,height);
    
}

void Robo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    int segmentos = (radius/3);
    if (segmentos < 16)
        segmentos = 16;
    glColor3f(1,1,1);

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0, 0); // centro
        for (int i = 0; i <= segmentos; i++) {
            if (i > 0){
                glColor3f(R,G,B);
            }
            float angulo = 2.0f * M_PI * i / segmentos;
            float x = 0 + radius * cosf(angulo);
            float y = 0 + radius * sinf(angulo);
            glVertex2f(x, y);
        }
    glEnd();
}

void Robo::DesenhaRoda(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat R, GLfloat G, GLfloat B)
{

}

void Robo::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    int h = 20;
    int l = 5; 

    glPushMatrix();
        glTranslatef(x, y+h+1, 0);
        glRotated(theta1,0,0,1);
        DesenhaRect(h,l,0.9,0.9,0.0); // amaelo

            glTranslatef(x, 2*h+1, 0);
            glRotated(theta2,0,0,1);
            DesenhaRect(h,l,0.0,0.9,0.9); // ciano

            glTranslatef(x, 2*h+1, 0);
            glRotated(theta3,0,0,1);
            DesenhaRect(h,l,0.9,0.0,0.9); // magenta

    glPopMatrix();
}

void Robo::DesenhaRobo(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{   
    int hCorpo = 50;
    int lCorpo = 100;

    // corpo
    glPushMatrix();
        glTranslatef(x, y+hCorpo, 0.0f);
        DesenhaRect(hCorpo,lCorpo,0.4,0.0,0.0);
    glPopMatrix();
    // braços
    DesenhaBraco(x,y+lCorpo,theta1,theta2,theta3);
    //rodas
    glPushMatrix();
        glTranslatef(x+lCorpo, y, 0.0f);
        glRotated(thetaWheel,0,0,1);
        DesenhaCirc(40,0.6,0.2,0.8);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(x-lCorpo, y, 0.0f);
        glRotated(thetaWheel,0,0,1);
        DesenhaCirc(40,0.6,0.2,0.8);
    glPopMatrix();

}

void Robo::RodaBraco1(GLfloat inc)
{

}

void Robo::RodaBraco2(GLfloat inc)
{

}

void Robo::RodaBraco3(GLfloat inc)
{

}

void Robo::MoveEmX(GLfloat dx)
{
}

//Funcao auxiliar de rotacao
void RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut){

}

Tiro* Robo::Atira()
{

}
