#ifndef COLIDIVEL_CILINDRO_H
#define COLIDIVEL_CILINDRO_H

#include "colidivel.h"
#include <math.h>
#include <GL/glut.h>

class ColidivelCilindro : public Colidivel {
protected:
    float cx, cy, cz;     // Centro no plano XY
    float raio;       // Raio do cilindro
    float altura;     // Altura (eixo Z)
    float anguloX,anguloY, anguloZ;

public:
    ColidivelCilindro(float x = 0, float y = 0, float r = 10, float h = 20)
        : cx(x), cy(y), raio(r), altura(h) {}

    // Implementação padrão da interface: retorna o AABB que envolve o cilindro
    // (útil se você ainda quiser usar a sua função ColideCom antiga em algum lugar)
    virtual void GetColider(GLfloat& x, GLfloat& y, GLfloat& w, GLfloat& h) const override {
        x = cx - raio;
        y = cy - raio;
        w = cx + raio;
        h = cy + raio;
    }

    // Nova função de colisão real (Círculo vs Círculo)
    // Usamos static para poder chamar sem um objeto específico
    static bool ColideCilindro(const ColidivelCilindro& a, const ColidivelCilindro& b) {
        float dx = a.cx - b.cx;
        float dy = a.cy - b.cy;
        float dist = sqrtf(dx*dx + dy*dy);
        return dist < (a.raio + b.raio);
    }

    // Funções para atualizar a posição do colisor
    void SetPosColisor(float x, float y, float z) { cx = x; cy = y; cz=z;}
    void SetRaio(float r) { raio = r; }
    void SetAltura(float h) { altura = h; }
    void SetRotateColisorX(float angGraus) { anguloX = angGraus; }
    void SetRotateColisorY(float angGraus) { anguloY = angGraus; }
    void SetRotateColisorZ(float angGraus) { anguloZ = angGraus; }
    // --- MODO DEBUG ---
    bool debugMode = false;
    void ToggleDebug() { debugMode = !debugMode; }

    void DesenhaDebug() const {
        if (!debugMode) return;

        glPushMatrix();
        // roda sobre Y
        glTranslatef(cx, cy, cz);
        glRotatef(anguloX, 1.0f, 0.0f, 0.0f); 
        glRotatef(anguloY, 0.0f, 1.0f, 0.0f); 
        glRotatef(anguloZ, 0.0f, 0.0f, 1.0f); 

        
        // Desabilita iluminação e textura para o debug ficar visível
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Desenha um cilindro wireframe verde translúcido
        glColor4f(0.0f, 1.0f, 0.0f, 0.5f); // Verde com 50% de opacidade
        GLUquadric* quad = gluNewQuadric();
        gluQuadricDrawStyle(quad, GLU_LINE); // Wireframe
        gluCylinder(quad, raio, raio, altura, 16, 4);
        gluDeleteQuadric(quad);

        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }
};

#endif