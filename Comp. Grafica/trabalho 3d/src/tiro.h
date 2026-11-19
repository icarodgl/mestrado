#ifndef TIRO_H
#define TIRO_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "colidivel_cilindro.h"
#include "configuracao.h"
#include "objloader.h"

class Tiro : public ColidivelCilindro
{
    GLfloat alturaMax  = 500;
    GLfloat larguraMax = 500;
    GLfloat gYaw;      // ângulo no plano XY (graus)
    GLfloat gPitch;    // inclinação vertical (graus)
    GLfloat gVel;

    static mesh  malhaBala;
    static bool  malhaCarregada;
    static float escalaMalha;
    static float minYMalha;

    void garanteMalha();

public:
    Tiro(GLfloat x, GLfloat y, GLfloat z,
         GLfloat yaw, GLfloat pitch,
         GLfloat velocidade)
        : ColidivelCilindro(x, y, z, /*r=*/10.0f, /*h=*/10.0f)
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