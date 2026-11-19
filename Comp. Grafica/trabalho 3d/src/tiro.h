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
    GLfloat gVel;

    // Direção normalizada no mundo
    GLfloat dirX, dirY, dirZ;

    static mesh  malhaBala;
    static bool  malhaCarregada;
    static float escalaMalha;
    static float minYMalha;

    void garanteMalha();

public:
    // Construtor único: direção VETORIAL (normalizada internamente)
    Tiro(GLfloat x, GLfloat y, GLfloat z,
         GLfloat dx, GLfloat dy, GLfloat dz,
         GLfloat velocidade)
        : ColidivelCilindro(x, y, z, /*r=*/10.0f, /*h=*/10.0f)
    {
        float len = sqrtf(dx*dx + dy*dy + dz*dz);
        if (len < 1e-6f) { dx = 0; dy = 1; dz = 0; len = 1; }
        dirX = dx / len;
        dirY = dy / len;
        dirZ = dz / len;
        gVel = velocidade;
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