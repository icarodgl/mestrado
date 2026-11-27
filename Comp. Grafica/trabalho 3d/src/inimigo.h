#ifndef INIMIGO_H
#define INIMIGO_H

#include <GL/gl.h>
#include "objloader.h"
#include "animation.h"
#include <vector>

class Inimigo {
private:
    // ---- Por instância: controlador de animação (leve) ----
    Animation m_anim;

    // ---- Estado por instância ----
    GLfloat cx = 0, cy = 0, cz = 0;
    float   armaAngulo = 0.0f;

    // ---- Recursos COMPARTILHADOS (estáticos) ----
    static GLuint s_texturaInimigo;
    static GLuint s_texturaArma;
    static bool   s_texturasCarregadas;
    static float  s_escala;

    static void carregaTexturas();

public:
    Inimigo();
    ~Inimigo() = default;   // não deleta estáticos

    // Liga ao vetor de frames compartilhado (chamado pelo Barril)
    void Configurar(const std::vector<mesh>* frames);

    void Atualiza(float dt);
    void SetPosicao(GLfloat x, GLfloat y, GLfloat z);
    void SetArmaLook(GLfloat anguloGrausMundo);
    void Desenha() const;

    GLfloat GetX() const { return cx; }
    GLfloat GetY() const { return cy; }
    GLfloat GetZ() const { return cz; }
};

#endif