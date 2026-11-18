#ifndef PERSONAGEM_H
#define PERSONAGEM_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "tiro.h"
#include "colidivel.h"
#include "configuracao.h"
#include "animation.h"
#include "colidivel_cilindro.h"

class Personagem : public ColidivelCilindro
{
    enum DirecaoMovimento
    {
        DIR_NENHUMA,
        DIR_FRENTE,
        DIR_TRAS,
        DIR_ESQ,
        DIR_DIR
    };
    int vida;
    GLfloat gX = 0;
    GLfloat gY = -180;
    GLfloat ultimoX = 0.0f;
    GLfloat ultimoY = 0.0f;
    GLfloat fasePasso = 0.0f;
    GLfloat theta = 0;
    GLfloat thetaArma = 0;
    GLfloat thetaArmaPitch = 0.0f; // pitch da arma (graus, -90..+90)
    GLfloat pernas = 40;
    GLfloat velocidade = 5.0;
    GLfloat raioCabeca = 30.0;
    // arena
    int larguraArena = 200;
    int alturaArena = 150;
    int CorrecaoArenaY = 200;
    // 3d
    Animation animAndandoFrente;
    Animation animAndandoTras;
    Animation animAndandoEsq;
    Animation animAndandoDir;
    Animation animParado;

    DirecaoMovimento direcaoAtual = DIR_NENHUMA;
    bool animCarregada = false;
    bool emMovimento = false;
    float tempoSemMover = 0.0f;

private:
    void DesenhaPersonagem(GLfloat x, GLfloat y);
    GLfloat CalculaBalancoPasso(GLfloat amplitude) const;
    void AtualizaAnimacao(GLfloat dt);

public:
    Personagem() : ColidivelCilindro(0, -180, 0, 10, 80) {};
    void GaranteAnimacao();                   // carrega lazily (primeira vez)
    void AtualizaAnimacaoAnim(float dt);      // avança e troca estado
    void SetDirecaoMovimento(int dx, int dy); // dx/dy em {-1, 0, 1}
    void Configurar(Configuracao conf)
    {
        if (conf.jogador.raioCabeca)
        {
            raioCabeca = conf.jogador.raioCabeca;
            pernas = raioCabeca * 1.4;
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
        GaranteAnimacao();
    }

    void Desenha()
    {
        DesenhaPersonagem(gX, gY);
    };
    bool Atingido(Colidivel *obj);
    void GirarArma(int x, int y);
    void Mover(GLfloat dir);
    void MoverLado(GLfloat dir);
    void Girar(GLfloat dir)
    {
        theta += dir * velocidade;
        SetRotateColisorZ(theta);
    }
    void getPos(GLfloat &x, GLfloat &y)
    {
        x = gX;
        y = gY;
    }
    GLfloat getAngulo() const { return theta; }
    void getDirecao(GLfloat &dx, GLfloat &dy) const
    {
        GLfloat rad = theta * M_PI / 180.0f;
        dx = -sinf(rad);
        dy = cosf(rad);
    }
    Tiro *Atirar(GLfloat velocidade);
    virtual void GetColider(GLfloat &x, GLfloat &y, GLfloat &w, GLfloat &h) const override
    {
        x = gX - raioCabeca;
        y = gY - raioCabeca;
        w = gX + raioCabeca;
        h = gY + raioCabeca;
    };
    void GirarArmaPitch(GLfloat dir)
    {
        thetaArmaPitch += dir;
        if (thetaArmaPitch > 60.0f)
            thetaArmaPitch = 60.0f;
        if (thetaArmaPitch < -45.0f)
            thetaArmaPitch = -45.0f;
    }
    // Define a mira 3D absoluta (usado em 1ª pessoa com unproject)
    void SetMira3D(float yawGraus, float pitchGraus)
    {
        theta = yawGraus;
        thetaArmaPitch = pitchGraus;
        SetRotateColisorZ(theta);
    }

    // Mira 2D: só yaw, zera pitch
    void SetMira3D(float alvoX, float alvoY, float /*ignorar*/)
    {
        float dx = alvoX - gX;
        float dy = alvoY - gY;
        float yaw = atan2f(dy, dx) * 180.0f / M_PI; // yaw "matemático" (0 => +X)
        // converte para o sistema do personagem (0 => +Y, frente)
        yaw -= 90.0f;
        while (yaw > 180.0f)
            yaw -= 360.0f;
        while (yaw < -180.0f)
            yaw += 360.0f;

        theta = yaw;
        thetaArmaPitch = 0.0f;
        SetRotateColisorZ(theta);
    }

    float getPitchArma() const { return thetaArmaPitch; }
};

#endif
