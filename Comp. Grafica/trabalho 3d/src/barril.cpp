#include "barril.h"
#include "tiro.h"
#include <math.h>
#include <stdio.h>
#include "texture.h"

// ---------- Estáticos do Barril ----------
mesh   Barril::malhaBarril;
bool   Barril::malhaCarregada     = false;
float  Barril::escalaMalha        = 48.0f;
float  Barril::minYMalha          = 0.0f;
GLuint Barril::texturaBarril      = 0;
bool   Barril::texturaCarregada   = false;

Barril::~Barril() {
    delete inimigo;
    inimigo = nullptr;
}

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
    if (!malhaBarril.loadMesh("src/assets/barril.obj"))
        fprintf(stderr, "Falha ao carregar barril.obj\n");
    malhaBarril.inverterTextura();
    malhaCarregada = true;
}

void Barril::Configurar(Configuracao conf, const std::vector<mesh>* frames) {
    if (conf.barril.velocidade) velocidade = conf.barril.velocidade;
    maxDistBarril = conf.arena.altura;

    if (temInimigo && frames && !frames->empty()) {
        if (!inimigo) inimigo = new Inimigo();
        inimigo->Configurar(frames);
    }
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
    if (!malhaBarril.vertsPos.empty()) {
        glPushMatrix();
        glTranslatef(cx, cy, cz);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        glRotatef(-anguloRotacao, 0.0f, 1.0f, 0.0f);
        glScalef(escalaMalha, escalaMalha, escalaMalha);
        glTranslatef(0.0f, -minYMalha, 0.0f);
        malhaBarril.draw(texturaBarril);
        glPopMatrix();
    }

    if (temInimigo && inimigo)
        inimigo->Desenha();

    DesenhaDebug();
}

void Barril::AtualizaInimigo(float dt, GLfloat lookX, GLfloat lookY, GLfloat lookZ) {
    if (!inimigo) return;

    float dx = lookX - cx;
    float dy = lookY - cy;
    float angulo = atan2f(dy, dx) * 180.0f / M_PI;

    inimigo->Atualiza(dt);
    inimigo->SetPosicao(cx, cy, cz);       // único lugar que seta a posição
    inimigo->SetArmaLook(angulo);
}

bool Barril::Valido() {
    return (vida > 0 && cy > -(maxDistBarril)/2);
}

void Barril::Atingido() { vida--; }

Tiro* Barril::Atirar(GLfloat, GLfloat) { return nullptr; }