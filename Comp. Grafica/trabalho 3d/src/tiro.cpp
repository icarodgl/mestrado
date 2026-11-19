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



void Tiro::Move(GLfloat delta)
{
    cx += dirX * gVel * delta;
    cy += dirY * gVel * delta;
    cz += dirZ * gVel * delta;
    SetPosColisor(cx, cy, cz);
}

void Tiro::Desenha()
{
    if (malhaBala.vertsPos.empty()) return;

    // Reconstrói yaw/pitch a partir da direção (para orientar a malha)
    float yawRad   = atan2f(-dirX, dirY);
    float pitchRad = asinf(fmaxf(-1.0f, fminf(1.0f, dirZ)));

    float yawDeg   = yawRad   * 180.0f / M_PI;
    float pitchDeg = pitchRad * 180.0f / M_PI;

    glPushMatrix();
    glTranslatef(cx, cy, cz + altura * 0.5f);

    glRotatef(yawDeg,   0.0f, 0.0f, 1.0f);
    glRotatef(pitchDeg, 1.0f, 0.0f, 0.0f);
    glRotatef(90.0f,    0.0f, 1.0f, 0.0f);

    glScalef(escalaMalha, escalaMalha, escalaMalha);
    glTranslatef(0.0f, -minYMalha, 0.0f);

    GLfloat matBala[] = { 1.0f, 0.8f, 0.2f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matBala);
    malhaBala.draw();
    glPopMatrix();
}


bool Tiro::Valido()
{
    if (cx > larguraMax || cx < -larguraMax) return false;
    if (cy > alturaMax  || cy < -alturaMax)  return false;
    if (cz > 500.0f || cz + altura < 0.0f)   return false;
    return true;
}