#ifndef ARENA_H
#define ARENA_H

#include <GL/gl.h>
#include "configuracao.h"
#include "objloader.h"

class Arena
{
private:
    float largura;
    float profundidade;
    float alturaParede;

    GLuint texturaArvore  = 0;
    GLuint texturaChao    = 0;

    GLuint texturasSkybox[6] = {0,0,0,0,0,0};
    GLuint texturaSkyboxEsfera = 0;
    bool   skyboxCarregado   = false;

    bool malhasCarregadas = false;

    mesh  malhaArvore;
    float escalaArvore    = 80.0f;
    float minYMalhaArvore = 0.0f;
    int   numArvores      = 12;

    mesh  malhaCone;
    float escalaCone    = 20.0f;
    float minYMalhaCone = 0.0f;
    int   numCones      = 12;

    


    void GaranteMalhas();
    void DesenhaCone(float x, float y);
    void DesenhaArvore(float x, float y, float angulo);
    void DesenhaLinhaDeConesArena() const;
    void DesenhaArvoresParedes() const;
    void DesenhaSkybox() const;
    void DesenhaSkyboxEsfera() const; 
    void CarregarSkybox();
public:
    Arena();
    ~Arena();

    void Configurar(const Configuracao &conf);
    void CarregarTexturas();
    void Desenha() const;
    void GetCentroArena(float &cx, float &cy) const { cx = 0.0f; cy = 0.0f; }
};

#endif