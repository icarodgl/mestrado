#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <GL/freeglut_std.h>
#include "configuracao.h"

class MyGameUI
{
    int vida;
    int pontos;
    int nParaGanhar;
    GLfloat vidaPosX = 180.0;
    GLfloat vidaPosY = 200.0;
 
private:
    void DesenhaPontos() const
    {
        std::string txt = "Pontos: " + std::to_string(pontos);
        glColor3f(1.0f, 1.0f, 1.0f);
        DesenhaTexto(-vidaPosX, vidaPosY, txt, GLUT_BITMAP_HELVETICA_18);
    }

    void DesenhaBarrisParaGanhar() const
    {
        std::string txt = "Faltam: " + std::to_string(nParaGanhar) + " Inimigos";
        glColor3f(1.0f, 1.0f, 1.0f);
        DesenhaTexto(-vidaPosX/3, vidaPosY, txt, GLUT_BITMAP_HELVETICA_18);
    }
    void DesenhaTexto(float x, float y, const std::string &texto, void *fonte) const
    {
        glRasterPos2f(x, y);
        for (char c : texto)
        {
            glutBitmapCharacter(fonte, c);
        }
    };
    void DesenhaGameOver()
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        std::string txt = "Game Over!";
        glPushMatrix();
        glTranslatef(0, 0, 0);
        glRecti(-250, -250, 250, 250);
        glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        DesenhaTexto(-20, 0, txt, GLUT_BITMAP_HELVETICA_18);
    }

    void DesenhaGanhou()
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        std::string txt = "WIN!";
        glPushMatrix();
        glTranslatef(0, 0, 0);
        glRecti(-250, -250, 250, 250);
        glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        DesenhaTexto(-20, 0, txt, GLUT_BITMAP_HELVETICA_18);
    }
    void DesenhaVida()
    {
        std::string txt = "Vidas: " + std::to_string(vida);
        glColor3f(1.0f, 1.0f, 1.0f);
        DesenhaTexto(vidaPosX, vidaPosY, txt, GLUT_BITMAP_HELVETICA_18);
    }

public:
    MyGameUI()
    {
        vida = 5;
        pontos = 0;
        nParaGanhar = 5;

    }
    void Atingido()
    {
        if (vida > 0)
            vida -= 1;
    };
    void Ponto()
    {
        if (vida > 0)
            pontos += 1;
    }
    void BarrilDestruido(){
        if (vida > 0)
            nParaGanhar -= 1;
    }
    void Desenha()
    {
        if (vida <= 0)
        {
            DesenhaGameOver();
        }
        if (nParaGanhar <= 0)
        {
            DesenhaGanhou();
        }
        DesenhaVida();
        DesenhaPontos();
        DesenhaBarrisParaGanhar();
    }
    void Configurar(Configuracao conf)
    {
        if (conf.jogador.vida)
            vida = conf.jogador.vida;
        if (conf.barril.nParaGanhar)
            nParaGanhar = conf.barril.nParaGanhar;
    }
};