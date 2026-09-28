#include "robo.h"
#include <math.h>
#include "tiro.h"


void Robo::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B)
{
    glColor3f(R,G,B);
    glRecti(0,0,width,height);
    
}

void Robo::DesenhaCirc(GLint radius, GLfloat R, GLfloat G, GLfloat B)
{
    int segmentos = (radius/3);
    if (segmentos < 24)
        segmentos = 24;
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
    glPushMatrix();
        glTranslatef(x, y+baseHeight, 0.0f);
        glRotated(thetaWheel,0,0,1);
        DesenhaCirc(radiusWheel,R,G,B);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(x+baseWidth, y+baseHeight, 0.0f);
        glRotated(thetaWheel,0,0,1);
        DesenhaCirc(radiusWheel,R,G,B);
    glPopMatrix();

}

void Robo::DesenhaBraco(GLfloat x, GLfloat y, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{
    int h = paddleHeight/2;
    int l = paddleWidth; 
    
    glPushMatrix();
        glTranslatef(x, y, 0);
        glRotated(theta1,0,0,1);
        DesenhaRect(h,l,0.9,0.9,0.0); // amaelo

            glTranslatef(0,h, 0);
            glRotated(theta2,0,0,1);
            DesenhaRect(h,l,0.0,0.9,0.9); // ciano

            glTranslatef(0,h, 0);
            glRotated(theta3,0,0,1);
            DesenhaRect(h,l,0.9,0.0,0.9); // magenta
    glPopMatrix();
}

void Robo::DesenhaRobo(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3)
{   
    // corpo
    glPushMatrix();
        glTranslatef(x, y+baseHeight, 0);
        DesenhaRect(baseHeight,baseWidth,0.4,0.0,0.0);
    glPopMatrix();
    // braços
    DesenhaBraco(x+(baseWidth/2),y+2*baseHeight,theta1,theta2,theta3);
    //rodas
    DesenhaRoda(x,y,thetaWheel,0.8,0.8,0.8);
}

void Robo::RodaBraco1(GLfloat inc)
{
    gTheta1 += inc;
}

void Robo::RodaBraco2(GLfloat inc)
{
    gTheta2 += inc;
}

void Robo::RodaBraco3(GLfloat inc)
{
    gTheta3+= inc;
}

void Robo::MoveEmX(GLfloat dx)
{   
    gX = gX + dx;
    gThetaWheel = gThetaWheel - dx;
}

//Funcao auxiliar de rotacao
void RotatePoint(GLfloat x, GLfloat y, GLfloat angle, GLfloat &xOut, GLfloat &yOut){
    GLfloat rad = angle * M_PI / 180.0f;
    xOut = -sinf(rad) * x;
    yOut = cosf(rad) * y;

}

Tiro* Robo::Atira()
{
    const GLfloat D2R = M_PI / 180.0f;
    const GLfloat h   = paddleHeight / 2.0f;

    GLfloat px = gX + baseWidth / 2.0f;
    GLfloat py = gY + 2 * baseHeight;   // mesmo offset do glTranslatef

    GLfloat a1 = gTheta1 * D2R;
    px += -sinf(a1) * h;
    py +=  cosf(a1) * h;

    GLfloat a2 = (gTheta1 + gTheta2) * D2R;
    px += -sinf(a2) * h;
    py +=  cosf(a2) * h;

    GLfloat a3 = (gTheta1 + gTheta2 + gTheta3) * D2R;
    px += -sinf(a3) * h;
    py +=  cosf(a3) * h;

    GLfloat angTotal = gTheta1 + gTheta2 + gTheta3;

    return new Tiro(px, py, angTotal);
}
