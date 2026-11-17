#include "barril.h"
#include "tiro.h"
#include <math.h>
#include <stdio.h>

// Inicialização das variáveis estáticas
mesh Barril::malhaBarril;
bool Barril::malhaCarregada = false;
float Barril::escalaMalha = 1.0f;
float Barril::minYMalha = 0.0f;

void Barril::garanteMalha() {
    if (malhaCarregada) return;

    // ATENÇÃO: Ajuste o caminho para o seu arquivo .obj real
    if (!malhaBarril.loadMesh("src/assets/barril.obj")) {
        fprintf(stderr, "Falha ao carregar barril.obj\n");
    }

    if (!malhaBarril.vertsPos.empty()) {
        // Calcula bounding box para escala e para apoiar no chão
        float mn[3] = { 1e30f, 1e30f, 1e30f};
        float mx[3] = {-1e30f,-1e30f,-1e30f};
        for (auto& v : malhaBarril.vertsPos) {
            mn[0]=fminf(mn[0],v.x); mn[1]=fminf(mn[1],v.y); mn[2]=fminf(mn[2],v.z);
            mx[0]=fmaxf(mx[0],v.x); mx[1]=fmaxf(mx[1],v.y); mx[2]=fmaxf(mx[2],v.z);
        }
        minYMalha = mn[1];

        // Escala: Queremos um barril com cerca de 40 unidades de altura
        float altura = mx[1] - mn[1];
        float maior = fmaxf(altura, fmaxf(mx[0]-mn[0], mx[2]-mn[2]));
        escalaMalha = (maior > 1e-4f) ? (140.0f / maior) : 1.0f;
    }
    malhaCarregada = true;
}

Barril::Barril(GLfloat _x, GLfloat _y, int _tipo) {
    x = _x; y = _y;
    tipo = _tipo;
    vida = 3;
    velocidade = 1.0f;
    anguloRotacao = 0.0f;
    garanteMalha();
}

void Barril::Configurar(Configuracao conf) {
    if (conf.barril.velocidade) {
        velocidade = conf.barril.velocidade * 1.0f; // Ajuste o multiplicador
    }
}

void Barril::Mirar(GLfloat px, GLfloat py) {
    // O barril não precisa "mirar" visualmente, mas guarda a direção se precisar
}

void Barril::Move(GLfloat dt) {
    // O barril desce em direção ao jogador (Y negativo)
    y -= velocidade * dt;
    
    // Gira o barril continuamente (velocidade do giro em graus/segundo)
    anguloRotacao += 150.0f * dt; 
    if (anguloRotacao > 360.0f) anguloRotacao -= 360.0f;
}

void Barril::Desenha() {
    if (malhaBarril.vertsPos.empty()) return;

    glPushMatrix();

    // 1) Posição na arena
    glTranslatef(x, y, 45.0);

    // 2) O Blender exporta Y-up. Deitamos o modelo no plano XY do jogo (-90 em X).
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

    // 3) Aplicar a rotação contínua do barril (agora no eixo Y, pois o modelo foi deitado)
    // Se você quiser que ele gire de forma "sambada" (inclinando), pode adicionar rotação em X ou Z aqui.
    glRotatef(-anguloRotacao, 0.0f, 1.0f, 0.0f); 

    // 4) Escala e apoio no chão
    glScalef(escalaMalha, escalaMalha, escalaMalha);
    glTranslatef(0.0f, -minYMalha, 0.0f);

    // 5) Desenha
    malhaBarril.draw();

    glPopMatrix();
}

bool Barril::Valido() {
    // Se sair da tela ou a vida acabar, ele deixa de ser válido
    return (vida > 0 && y > -200.0f);
}

void Barril::Atingido() {
    vida--;
}

Tiro* Barril::Atirar(GLfloat tempo, GLfloat velocidadeTiro) {
    // O barril pode atirar na direção do jogador, mas com um cooldown
    static float ultimoTiro = 0.0f;
    if (tempo - ultimoTiro > 2000.0f) { // Atira a cada 2 segundos
        ultimoTiro = tempo;
        // Calcula o ângulo para o jogador (assumindo que ele esteja em 0,-180)
        float angulo = atan2f(-180.0f - y, 0.0f - x) * 180.0f / M_PI;
        return new Tiro(x, y, angulo, velocidadeTiro);
    }
    return nullptr;
}