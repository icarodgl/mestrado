#include "barril.h"
#include "tiro.h"
#include <math.h>
#include <stdio.h>
#include "texture.h"   

// Inicialização das variáveis estáticas
mesh Barril::malhaBarril;
bool Barril::malhaCarregada = false;
float Barril::escalaMalha = 48.0f;
float Barril::minYMalha = 0.0f;
GLuint Barril::texturaBarril   = 0;       
bool   Barril::texturaCarregada = false;  


void Barril::garanteTextura() {
    if (texturaCarregada) return;

    texturaBarril = loadBMP("src/assets/barril.bmp");
    if (texturaBarril == 0)
        fprintf(stderr, "Barril: FALHA ao carregar barril.bmp\n");
    else
        printf("Barril: textura OK texId=%u\n", texturaBarril);

    texturaCarregada = true;
}
void Barril::garanteMalha() {
    if (malhaCarregada) return;

    // ATENÇÃO: Ajuste o caminho para o seu arquivo .obj real
    if (!malhaBarril.loadMesh("src/assets/barril.obj")) {
        fprintf(stderr, "Falha ao carregar barril.obj\n");
    }
    malhaBarril.inverterTextura();
    malhaCarregada = true;
}


void Barril::Configurar(Configuracao conf) {
    if (conf.barril.velocidade) {
        velocidade = conf.barril.velocidade; // Ajuste o multiplicador
    }
    maxDistBarril = conf.arena.altura;
}

void Barril::Mirar(GLfloat px, GLfloat py) {
    // O barril não precisa "mirar" visualmente, mas guarda a direção se precisar
}

void Barril::Move(GLfloat dt) {
    if (!isfinite(dt) || dt <= 0.0f) dt = 1;
    cy -= velocidade * dt;
    anguloRotacao += 150.0f * dt; 
    if (anguloRotacao > 360.0f) anguloRotacao -= 360.0f;
    SetRotateColisorY(-90);
    SetRotateColisorZ(-anguloRotacao); 
}

void Barril::Desenha() {
    if (malhaBarril.vertsPos.empty()) return;

    glPushMatrix();
    // 1) Posição na arena
    glTranslatef(cx, cy, cz);
    // 2) Deitar o modelo Y-up no plano XY do jogo
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    // 3) Rotação contínua do barril
    glRotatef(-anguloRotacao, 0.0f, 1.0f, 0.0f);

    // 4) Escala e apoio no chão
    glScalef(escalaMalha, escalaMalha, escalaMalha);
    glTranslatef(0.0f, -minYMalha, 0.0f);
    malhaBarril.draw(texturaBarril);
    glPopMatrix();
    DesenhaDebug();
}

bool Barril::Valido() {
    // Se sair da tela ou a vida acabar, ele deixa de ser válido
    return (vida > 0 && cy > -(maxDistBarril)/2);
}

void Barril::Atingido() {
    vida--;
}

Tiro* Barril::Atirar(GLfloat tempo, GLfloat velocidadeTiro) {
    // O barril pode atirar na direção do jogador, mas com um cooldown
    static float ultimoTiro = 0.0f;
    // if (tempo - ultimoTiro > 2000.0f) { // Atira a cada 2 segundos
    //     ultimoTiro = tempo;
    //     // Calcula o ângulo para o jogador (assumindo que ele esteja em 0,-180)
    //     float angulo = atan2f(-180.0f - y, 0.0f - x) * 180.0f / M_PI;
    //     return new Tiro(x, y,50, angulo,0.0, velocidadeTiro);
    // }
    return nullptr;
}