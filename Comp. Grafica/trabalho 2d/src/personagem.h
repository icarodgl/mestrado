#ifndef PERSONAGEM_H
#define PERSONAGEM_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "tiro.h"
#include "colidivel.h"
#include "configuracao.h"

class Personagem : public Colidivel
{
    int vida;
    GLfloat gX = 0;
    GLfloat gY = -180;
    GLfloat theta = 0;
    GLfloat pernas = 40;
    GLfloat velocidade = 5.0;
    GLfloat raioCabeca = 30.0;
    // arena
    int larguraArena = 200;
    int alturaArena = 150;
    int CorrecaoArenaY = 200;

private:
    void DesenhaPersonagem(GLfloat x, GLfloat y);
    void DesenhaArena();

public:
    Personagem() {};
    void Configurar(Configuracao conf)
    {
        if (conf.jogador.raioCabeca)
        {
            raioCabeca = conf.jogador.raioCabeca;
        }
        if (conf.jogador.velocidade)
        {
            velocidade = conf.jogador.velocidade;
        }
        if (conf.jogador.vida)
        {
            vida = conf.jogador.vida;
        }
        if (conf.arena.altura and conf.arena.altura >= 100)
        {
            alturaArena = conf.arena.altura;

        }
        if (conf.arena.largura and conf.arena.largura >= 100)
        {
            larguraArena = conf.arena.largura;
        }
    }

    void Desenha()
    {
        DesenhaPersonagem(gX, gY);
    };
    bool Atingido(Colidivel *obj);
    void Girar(int x, int y);
    void Mover(GLfloat dir);
    void MoverLado(GLfloat dir);
    void getPos(GLfloat &x, GLfloat &y){
        x = gX;
        y = gY;
    }
    Tiro *Atirar(GLfloat velocidade);
    virtual void GetColider(GLfloat &x, GLfloat &y, GLfloat &w, GLfloat &h) const override
    {
        x = gX - raioCabeca;
        y = gY - raioCabeca;
        w = gX + raioCabeca;
        h = gY + raioCabeca;
    };
};

#endif
