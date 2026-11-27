#include "inimigo.h"
#include "texture.h"
#include <math.h>
#include <stdio.h>


// ---------- Estáticos ----------
GLuint Inimigo::s_texturaInimigo   = 0;
GLuint Inimigo::s_texturaArma      = 0;
bool   Inimigo::s_texturasCarregadas = false;
float  Inimigo::s_escala           = 10.0f;   // ajuste conforme seu OBJ

void Inimigo::carregaTexturas() {
    if (s_texturasCarregadas) return;

    s_texturaInimigo = loadBMP("src/assets/inimigo/inimigo.bmp");
    if (s_texturaInimigo == 0)
        fprintf(stderr, "Inimigo: falha textura inimigo\n");

    s_texturaArma = loadBMP("src/assets/arma.bmp");
    if (s_texturaArma == 0)
        fprintf(stderr, "Inimigo: falha textura arma (opcional)\n");

    s_texturasCarregadas = true;
}

Inimigo::Inimigo() {}

void Inimigo::Configurar(const std::vector<mesh>* frames) {
    carregaTexturas();
    m_anim.setFrames(frames);
    m_anim.setFPS(20.0f);
    m_anim.setLoop(true);
}

void Inimigo::Atualiza(float dt) {m_anim.update(dt); }

void Inimigo::SetPosicao(GLfloat x, GLfloat y, GLfloat z) { cx = x; cy = y; cz = z; }
void Inimigo::SetArmaLook(GLfloat a) { armaAngulo = a; }

void Inimigo::Desenha() const {
    if (m_anim.empty()) return;

    glPushMatrix();
    glTranslatef(cx, cy, cz);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);   // Y-up do Blender → XY do jogo
    glScalef(s_escala, s_escala, s_escala);

    if (s_texturaInimigo != 0) {
        texture_enable();
        glBindTexture(GL_TEXTURE_2D, s_texturaInimigo);
    }
    m_anim.draw(s_texturaInimigo);
    if (s_texturaInimigo != 0) texture_disable();

    glPopMatrix();

    // (Se quiser arma: adicione o desenho dela aqui, usando armaAngulo)
}