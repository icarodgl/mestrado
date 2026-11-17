#ifndef ARENA_H
#define ARENA_H

#include <GL/gl.h>
#include "configuracao.h"
#include "objloader.h"

class Arena {
private:
    float largura;
    float profundidade;
    float alturaParede;

    GLuint texturaParede = 0;
    GLuint texturaChao   = 0;

    // NOVO: Malha do cone e parâmetros
    mesh   malhaCone;
    bool   coneCarregado = false;
    float  raioAreaCentral = 80.0f;   // Raio da área onde o jogador NÃO pode entrar
    float  escalaCone      = 1.0f;
    float  minYMalhaCone   = 0.0f;
    int    numCones        = 12;      // Quantos cones ao redor do círculo

    // Método interno para garantir que a malha do cone foi carregada
    void GaranteMalhaCone();
    // Desenha um único cone na posição (x, y)
    void DesenhaCone(float x, float y);
    void DesenhaLinhaDeConesArena()const;
public:
    Arena();
    ~Arena();

    void Configurar(const Configuracao& conf);
    void CarregarTexturas();   // Agora também carrega a malha do cone
    
    void Desenha() const;

    // NOVO: Retorna o raio da área central (para o personagem saber onde parar)
    float GetRaioAreaCentral() const { return raioAreaCentral; }
    // NOVO: Retorna o centro da arena (sempre 0,0 no seu caso)
    void GetCentroArena(float& cx, float& cy) const { cx = 0.0f; cy = 0.0f; }
};

#endif