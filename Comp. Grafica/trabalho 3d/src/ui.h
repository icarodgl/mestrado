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
    GLfloat altura;
    GLfloat largura; 
    GLfloat vidaPosX = 200.0;
    GLfloat vidaPosY = 200.0;
 
private:
    void DesenhaVida()
    {
        std::string txt = "Vidas: " + std::to_string(vida);
        glColor3f(1.0f, 1.0f, 1.0f);
        DesenhaTexto(vidaPosX-80, vidaPosY-50, txt, GLUT_BITMAP_HELVETICA_18);
    }
    void DesenhaPontos() const
    {
        std::string txt = "Pontos: " + std::to_string(pontos);
        glColor3f(1.0f, 1.0f, 1.0f);
        DesenhaTexto(-vidaPosX, vidaPosY-50, txt, GLUT_BITMAP_HELVETICA_18);
    }

    void DesenhaBarrisParaGanhar() const
    {
        std::string txt = "Faltam: " + std::to_string(nParaGanhar) + " Inimigos";
        glColor3f(1.0f, 1.0f, 1.0f);
        DesenhaTexto(-vidaPosX/3, vidaPosY-50, txt, GLUT_BITMAP_HELVETICA_18);
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
        std::string txtInfo = "Aperte R para reiniciar o jogo";

        glPushMatrix();
        glTranslatef(-largura,-altura, 0);
        glRecti(0, 0, largura*2, altura*2 );
        glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        DesenhaTexto(-50, 0, txt, GLUT_BITMAP_HELVETICA_18);
        DesenhaTexto(-80, -30, txtInfo, GLUT_BITMAP_HELVETICA_12);

    }

    void DesenhaGanhou()
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        std::string txt = "WIN!";
        std::string txtInfo = "Aperte R para reiniciar o jogo";
        glPushMatrix();
        glTranslatef(-largura,-altura, 0);
        glRecti(0, 0, largura*2, altura*2 );
        glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        DesenhaTexto(-20, 0, txt, GLUT_BITMAP_HELVETICA_18);
        DesenhaTexto(-80, -30, txtInfo, GLUT_BITMAP_HELVETICA_12);
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
    void Reset(Configuracao conf){
        pontos = 0;
        if (conf.jogador.vida)
            vida = conf.jogador.vida;
        if (conf.barril.nParaGanhar)
            nParaGanhar = conf.barril.nParaGanhar;
    }
    void Configurar(Configuracao conf)
    {
        if (conf.jogador.vida)
            vida = conf.jogador.vida;
        if (conf.barril.nParaGanhar)
            nParaGanhar = conf.barril.nParaGanhar;
        if (conf.arena.altura){
            altura = conf.arena.altura;
            vidaPosY = conf.arena.altura/2;
        }
        if (conf.arena.largura){
            largura = conf.arena.largura;
            vidaPosX = conf.arena.largura/2;
        }
    }
};