#ifndef BARRIL_H
#define BARRIL_H

#include <GL/gl.h>
#include "colidivel.h"
#include "configuracao.h"
#include "objloader.h"
#include "tiro.h"

class Barril : public Colidivel {
private:
    GLfloat x, y;
    GLfloat velocidade;
    GLfloat anguloRotacao; // NOVO: para fazer o barril girar
    int vida;
    int tipo; // 0 ou 1 (usado para textura/cor, se quiser diferenciar)

    // Variáveis estáticas para carregar a malha apenas uma vez
    static mesh malhaBarril;
    static bool malhaCarregada;
    static float escalaMalha;
    static float minYMalha;

    void garanteMalha();

public:
    Barril(GLfloat _x, GLfloat _y, int _tipo);
    void Configurar(Configuracao conf);
    void Mirar(GLfloat px, GLfloat py);
    void Move(GLfloat dt);
    void Desenha();
    bool Valido();
    void Atingido();
    Tiro* Atirar(GLfloat tempo, GLfloat velocidadeTiro);
    
    virtual void GetColider(GLfloat &x, GLfloat &y, GLfloat &w, GLfloat &h) const override {
        x = this->x - 20.0f; 
        y = this->y - 20.0f;
        w = this->x + 20.0f;
        h = this->y + 20.0f;
    }
};

#endif