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
    GLfloat tamanho = 30.0;
    GLfloat theta = 0;
    GLfloat pernas = 40;
    GLfloat velocidade = 5.0;
    GLfloat raioCabeca = 30.0;
    // arena
    int larguraArena = 200;
    int alturaArena = 150;
    int CorrecaoArenaY = 100;

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
            gY = -((conf.arena.altura - raioCabeca) + CorrecaoArenaY);
        }
        if (conf.arena.largura and conf.arena.largura >= 100)
        {
            larguraArena = conf.arena.largura;
            gX = (conf.arena.largura) / 2;
        }
    }

    void Desenha()
    {
        DesenhaPersonagem(gX, gY);
    };
    bool Atingido(Colidivel *obj);
    void Girar(int dir);
    void Mover(int dir);
    void MoverLado(int dir);
    Tiro *Atirar(GLfloat velocidade);
    virtual void GetColider(GLfloat &x, GLfloat &y, GLfloat &w, GLfloat &h) const override
    {
        x = gX - tamanho;
        y = gY - tamanho;
        w = gX + tamanho;
        h = gY + tamanho;
    };
};

#endif
