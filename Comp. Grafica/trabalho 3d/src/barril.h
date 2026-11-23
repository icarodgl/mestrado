#ifndef BARRIL_H
#define BARRIL_H

#include <GL/gl.h>
#include "configuracao.h"
#include "objloader.h"
#include "tiro.h"
#include "colidivel_cilindro.h"

class Barril : public ColidivelCilindro {
private:
    GLfloat velocidade;
    GLfloat anguloRotacao; // NOVO: para fazer o barril girar
    int vida;
    int tipo; // 0 ou 1 (usado para textura/cor, se quiser diferenciar)
    float maxDistBarril = 2000;

    // Variáveis estáticas para carregar a malha apenas uma vez
    static mesh malhaBarril;
    static bool malhaCarregada;
    static float escalaMalha;
    static float minYMalha;

    void garanteMalha();

public:
    Barril(GLfloat _x, GLfloat _y, int _tipo) : ColidivelCilindro(_x, _y,0, 50.0f, 140.0f){
    cx = _x; cy = _y; 
    cz = 45.0;
    tipo = _tipo;
    vida = 3;
    velocidade = 1.0f;
    anguloRotacao = 0.0f;
    garanteMalha();
    // SetRotateColisor(90.0f);
    }
    void Configurar(Configuracao conf);
    void Mirar(GLfloat px, GLfloat py);
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
    void GetPosition(GLfloat &outX, GLfloat &outY, GLfloat &outZ){ outX = cx; outY = cy; outZ = cz; }
};

#endif