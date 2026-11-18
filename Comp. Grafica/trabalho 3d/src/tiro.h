#ifndef TIRO_H
#define TIRO_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "colidivel_cilindro.h"   // ← troca de colidivel.h
#include "configuracao.h"
#include "objloader.h"

class Tiro : public ColidivelCilindro
{
    GLfloat alturaMax  = 500;
    GLfloat larguraMax = 500;
    GLfloat gYaw;                // ângulo no plano XY
    GLfloat gPitch;              // inclinação vertical (graus)
    GLfloat gVel;

    static mesh  malhaBala;
    static bool  malhaCarregada;
    static float escalaMalha;
    static float minYMalha;

    void garanteMalha();

public:
    // Construtor novo: recebe também o Z inicial e o pitch
    Tiro(GLfloat x, GLfloat y, GLfloat z,           // posição inicial (base do cilindro)
         GLfloat yaw, GLfloat pitch,                // direção
         GLfloat velocidade)
        : ColidivelCilindro(x, y,z,/*r=*/5.0f, /*h=*/5.0f)
    {
        gYaw   = yaw;
        gPitch = pitch;
        gVel   = velocidade;
        garanteMalha();
    }

    void Desenha();
    void Move(GLfloat deltaTime);
    bool Valido();
    void Configurar(Configuracao conf) {
        alturaMax  = conf.arena.altura;
        larguraMax = conf.arena.largura;
    }

    virtual void GetColider(GLfloat& x, GLfloat& y, GLfloat& w, GLfloat& h) const override {
        x = cx - raio; y = cy - raio;
        w = cx + raio; h = cy + raio;
    }
};
#endif