#ifndef BARRIL_H
#define BARRIL_H

#include <GL/gl.h>
#include "configuracao.h"
#include "objloader.h"
#include "tiro.h"
#include "colidivel_cilindro.h"
#include "inimigo.h"
#include <vector>

class Barril : public ColidivelCilindro {
private:
    GLfloat velocidade;
    GLfloat anguloRotacao;
    int vida;
    float maxDistBarril = 2000;

    static mesh malhaBarril;
    static bool malhaCarregada;
    static float escalaMalha;
    static float minYMalha;
    static GLuint texturaBarril;
    static bool   texturaCarregada;

    void garanteMalha();
    void garanteTextura();

    bool     temInimigo;
    Inimigo* inimigo = nullptr;

public:
    ~Barril();

    // _temInimigo: shadowing corrigido
    Barril(GLfloat _x, GLfloat _y, bool _temInimigo)
        : ColidivelCilindro(_x, _y, 0, 50.0f, 140.0f)
    {
        cx = _x; cy = _y; cz = 45.0;
        temInimigo = _temInimigo;
        vida = 3;
        velocidade = 1.0f;
        anguloRotacao = 0.0f;
        garanteMalha();
        garanteTextura();
    }

    // Recebe o vetor de frames COMPARTILHADO (não o Animation)
    void Configurar(Configuracao conf, const std::vector<mesh>* frames);

    void Move(GLfloat dt);
    void Desenha();
    bool Valido();
    void Atingido();
    Tiro* Atirar(GLfloat tempo, GLfloat velocidadeTiro);

    virtual void GetColider(GLfloat &x, GLfloat &y, GLfloat &w, GLfloat &h) const override {
        x = this->cx - 20.0f;
        y = this->cy - 20.0f;
        w = this->cx + 20.0f;
        h = this->cy + 20.0f;
    }
    void GetPosition(GLfloat &x, GLfloat &y, GLfloat &z) { x=cx; y=cy; z=cz; }

    void AtualizaInimigo(float dt, GLfloat lookX, GLfloat lookY, GLfloat lookZ);
};

#endif