#include <math.h>
#include <stdio.h>
#include <vector>
#include "personagem.h"
#include "tiro.h"
#include "utils.h"

using namespace std;


void Personagem::DesenhaArena()
    {
    // desenha somente a linha pontilhada no centro da arena
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);   // padrão: 8 bits acesos, 8 apagados

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.0f);

    glBegin(GL_LINES);
        glVertex2f(-larguraArena  / 2.0f, 0.0f);
        glVertex2f( larguraArena  / 2.0f, 0.0f);
    glEnd();

    glDisable(GL_LINE_STIPPLE);

    //desenha retangulo na arena onde o personagem pode andar
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
        
    glTranslatef(-larguraArena/2, -alturaArena/2, 0);
    glRecti(0, 0, larguraArena, alturaArena/2);
    glPopMatrix();
    }
void Personagem::DesenhaPersonagem(GLfloat x, GLfloat y)
{

    vector<GLfloat> CorArma = {0.75, 0.156, 0.09};
    vector<GLfloat> CorCorpo  = {0.161, 0.6, 0.45};
    vector<GLfloat> CorPernas = {0.9, 0.9, 0.9};
    GLfloat tamanhoPerna = raioCabeca*1.4;

    DesenhaArena();
    controlaPernas(tamanhoPerna, x, y, theta);

    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(theta, 0, 0, 1); // correções para centralizar no boneco


    glColor3f(CorPernas[0], CorPernas[1], CorPernas[2]);
   // perna direita
    glPushMatrix();
    glTranslatef(raioCabeca*0.6, 0, 0);
    glRecti(0, 0, raioCabeca/3, tamanhoPerna);
    glPopMatrix();

    // perna esquerda
    glPushMatrix();
    glTranslatef(-(raioCabeca*0.95), 0, 0);
    glRecti(0, 0, raioCabeca/3, -tamanhoPerna);
    glPopMatrix();

    // corpo
    glColor3f(CorCorpo[0], CorCorpo[1], CorCorpo[2]);
    glPushMatrix();
    DesenhaCirculo(0, 0,raioCabeca);
    glPopMatrix();

    // arma
    glColor3f(CorArma[0], CorArma[1], CorArma[2]);
    glPushMatrix();
    glRotatef(thetaArma, 0, 0, 1);
    glTranslatef(raioCabeca, 0, 0);
    glRecti(0, 0, raioCabeca*0.4, raioCabeca*1.8);
    glPopMatrix();

    glPopMatrix();
}
void Personagem::GirarArma(int x, int y)
{   
    GLfloat dx = x - gX;
    GLfloat dy = y - gY;
    GLfloat angulo = (atan2f(dy, dx) * 180.0f / M_PI)-90;
    if (angulo > 30 or  angulo < -30)
        return;
    thetaArma = angulo;
}

void Personagem::Mover(GLfloat dir)
{
    GLfloat rad = this->theta * M_PI / 180.0f;
    GLfloat vx = -sinf(rad) * (dir * velocidade);
    GLfloat vy = cosf(rad) * (dir * velocidade);

    // arena Altura
    if ((gY > ((0 - raioCabeca)) and vy > 0) or (gY < -((alturaArena/2)-raioCabeca)  and vy < 0))
        return;

    // arena Largura
    if ((gX > ((larguraArena/2)-raioCabeca) and vx > 0) or (gX < -((larguraArena/2) - raioCabeca) and vx < 0))
        return;

    this->gX += vx;
    this->gY += vy;
}
void Personagem::MoverLado(GLfloat dir)
{
    if ((gX > larguraArena-raioCabeca and dir > 0) or (gX < -larguraArena+raioCabeca and dir < 0))
        return;
    gX += dir*velocidade;
}
Tiro *Personagem::Atirar(GLfloat VelocidadeTiro)
{

    const GLfloat lx = raioCabeca + (raioCabeca*0.4) - ((raioCabeca*0.4)/2); // braco + centro canhao
    const GLfloat ly = raioCabeca*1.8; // ponta canhao

    const GLfloat DegToRad = M_PI / 180.0f; // Graus pra Radiano
    const GLfloat rad = (thetaArma + theta )* DegToRad;

    const GLfloat c = cosf(rad);
    const GLfloat s = sinf(rad);

    const GLfloat x = gX + lx * c - ly * s;
    const GLfloat y = gY + lx * s + ly * c;

    Tiro *t = new Tiro(x, y, (thetaArma + theta ), VelocidadeTiro);

    return t;
}

bool Personagem::Atingido(Colidivel *obj)
{
    if (!obj)
        return false;
    return ColideCom(*obj);
}