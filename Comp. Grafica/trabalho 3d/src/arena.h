#ifndef ARENA_H
#define ARENA_H

#include <GL/gl.h>
#include "configuracao.h"

class Arena {
private:
    float largura;
    float profundidade;
    float alturaParede;

    GLuint texturaParede = 0;
    GLuint texturaChao   = 0;

public:
    Arena();
    ~Arena();

    void Configurar(const Configuracao& conf);
    void CarregarTexturas();
    void Desenha() const;
};

#endif