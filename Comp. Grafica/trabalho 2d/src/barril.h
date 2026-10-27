#ifndef BARRIL_H
#define BARRIL_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include "tiro.h"
#include "colidivel.h"
#include "configuracao.h"

#define COL_X 10.0
#define COL_Y 10.0
#define DISTANCIA_MAX 400.0

class Barril : public Colidivel
{
    int vida;
    GLfloat height = 80.0;
    GLfloat width = 160.0;
    GLfloat gX;
    GLfloat gY;
    GLfloat velocidadeInimigo = 100.0;
    GLfloat theta = 180.0;
    GLfloat disparo = 0.0;
    GLfloat tiroPorSegundo = 1;
    GLfloat raioCabeca = 30.0;
    bool inimigo = false;

private:
    void DesenhaVida() const
    {
        std::string txt = std::to_string(vida);
    }
    void DesenhaBarril(GLfloat x, GLfloat y);
    void DesenhaInimigo(GLfloat x, GLfloat y);
    void DesenhaTexto(float x, float y, const std::string &texto, void *fonte) const
    {
        glRasterPos2f(x, y - height);
        for (char c : texto)
            glutBitmapCharacter(fonte, c);
    };

public:
    Barril(GLfloat x, GLfloat y, bool inimigo)
    {
        gX = x - (width / 2); // correção da metade do tamanho do barril para centralizar
        gY = y;
        this->vida = vida;
        this->inimigo = inimigo;
    };
    void Desenha()
    {
        DesenhaBarril(gX, gY);
        DesenhaVida();
        if (inimigo)
            DesenhaInimigo(gX + (width / 2), gY + (height / 2));
    };
    void Atingido()
    {
        vida -= 1;
    };

    void AtingidoPersonagem()
    {
        vida = 0;
    };
    void Move(GLfloat deltaTime)
    {
        gY -= velocidadeInimigo * deltaTime;
    }

    bool Valido()
    {
        if (vida <= 0)
            return false;
        if (gX > DISTANCIA_MAX or gX < -DISTANCIA_MAX or gY > DISTANCIA_MAX or gY < -DISTANCIA_MAX)
            return false;
        return true;
    }

    Tiro *Atirar(GLfloat deltaTime, GLfloat velocidadeTiro);

    void DesenhaVida()
    {
        std::string txt = std::to_string(vida);
        glColor3f(1.0f, 1.0f, 1.0f);
        // GLfloat x,y,h,w;
        // GetColider(x,y,w,h);
        // std::string pos = "theta"+std::to_string(theta);
        // std::string col = "w h "+std::to_string(w)+ " " + std::to_string(h);
        // DesenhaTexto(gX, gY + height - 10, pos, GLUT_BITMAP_HELVETICA_12);
        // DesenhaTexto(gX, gY + height - 20, pos, GLUT_BITMAP_HELVETICA_12);

        DesenhaTexto(gX, gY + height, txt, GLUT_BITMAP_HELVETICA_18);
    }
    virtual void GetColider(GLfloat &x, GLfloat &y, GLfloat &w, GLfloat &h) const override
    {
        x = gX;
        y = gY;
        w = gX + width;
        h = gY + height;
    };
    void Mirar(GLfloat x, GLfloat y)
    {
        GLfloat dx = x - gX;
        GLfloat dy = y - gY;
        GLfloat angulo = (atan2f(dy, dx) * 180.0f / M_PI);
        angulo -= 90.0f;
        const GLfloat MIN = -210.0f;
        const GLfloat MAX = -150.0f;

        if (angulo >= MIN && angulo <= MAX)
            theta = angulo;
    }
    bool getPossuiInimigo() { return this->inimigo; }
    void Configurar(Configuracao config)
    {
        if (config.inimigo.raioCabeca)
            raioCabeca = config.inimigo.raioCabeca;
        if (config.barril.altura)
            height = config.barril.altura;
        if (config.barril.largura)
            width = config.barril.largura;
        if (config.barril.velocidade)
            velocidadeInimigo = config.barril.velocidade;
        if (config.inimigo.tirosPorSegundo)
            tiroPorSegundo = config.inimigo.tirosPorSegundo;
        if (config.barril.vida)
            vida = this->inimigo ? config.barril.vida + 2 : config.barril.vida;
    }
};

#endif
