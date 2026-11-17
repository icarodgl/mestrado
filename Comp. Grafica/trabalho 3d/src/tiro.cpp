#include "tiro.h"
#include <math.h>
#include <stdio.h>

// Inicialização das variáveis estáticas
mesh Tiro::malhaBala;
bool Tiro::malhaCarregada = false;
float Tiro::escalaMalha = 1.0f;
float Tiro::minYMalha = 0.0f;

void Tiro::garanteMalha() {
    if (malhaCarregada) return;

    // ATENÇÃO: Ajuste o caminho se necessário
    if (!malhaBala.loadMesh("src/assets/bala.obj")) {
        fprintf(stderr, "Falha ao carregar bala.obj\n");
    }

    if (!malhaBala.vertsPos.empty()) {
        // Calcula bounding box
        float mn[3] = { 1e30f, 1e30f, 1e30f};
        float mx[3] = {-1e30f,-1e30f,-1e30f};
        for (auto& v : malhaBala.vertsPos) {
            mn[0]=fminf(mn[0],v.x); mn[1]=fminf(mn[1],v.y); mn[2]=fminf(mn[2],v.z);
            mx[0]=fmaxf(mx[0],v.x); mx[1]=fmaxf(mx[1],v.y); mx[2]=fmaxf(mx[2],v.z);
        }
        minYMalha = mn[1];

        // Escala para a bala ficar com ~15 unidades de comprimento
        // Ajuste esse valor para deixar a bala maior ou menor
        float altura = mx[1] - mn[1];
        float maior = fmaxf(altura, fmaxf(mx[0]-mn[0], mx[2]-mn[2]));
        escalaMalha = (maior > 1e-4f) ? (15.0f / maior) : 1.0f;
    }
    malhaCarregada = true;
}

void Tiro::Desenha() {
    if (malhaBala.vertsPos.empty()) return;

    glPushMatrix();

    // 1. Translada para a posição atual da bala no plano 2D
    glTranslatef(gX, gY, 50.0f);

    // 2. Gira a bala no plano XY para apontar na direção correta do tiro
    glRotatef(gDirectionAng, 0.0f, 0.0f, 1.0f);

    // 3. Deita o modelo do Blender (Y-up) no plano XY do jogo
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    // 4. Escala e ajuste para o centro da bala
    glScalef(escalaMalha, escalaMalha, escalaMalha);
    glTranslatef(0.0f, -minYMalha, 0.0f);

    // 5. Material da bala (amarelo/dourado, por exemplo)
    GLfloat matBala[] = { 1.0f, 0.8f, 0.2f, 1.0f }; 
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matBala);

    // 6. Desenha a malha
    malhaBala.draw();

    glPopMatrix();
}

void Tiro::Move(GLfloat delta) 
{   
    GLfloat rad = this->gDirectionAng * M_PI / 180.0f;
    GLfloat vx = -sinf(rad) * this->gVel * delta;
    GLfloat vy = cosf(rad) * this->gVel * delta;

    this->gX += vx;
    this->gY += vy;
}

bool Tiro::Valido()
{
    if (gX > larguraMax or gX < -larguraMax or gY > alturaMax or gY < -alturaMax)
        return false;
    return true;
}