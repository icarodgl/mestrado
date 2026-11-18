#include "camera.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static CameraMode s_mode = CAM_FIRST_PERSON;
static float s_distance = 1000.0f;
static float s_angleXZ = 90.0f;
static float s_angleXY = 30.0f;
static float s_targetX = 0.0f;
static float s_targetY = 0.0f;
static float s_targetZ = 0.0f;
static float s_baseHeight = 100.0f; // Altura regulável para a câmera 4
static float s_personagemAngulo = 0.0f;
static float s_pitch = 0.0f; // pitch em graus (-89..+89)

void camera_init(void)
{
    s_mode = CAM_FIRST_PERSON;
    s_distance = 900.0f;
    s_angleXZ = 90.0f;
    s_angleXY = 30.0f;
    s_targetX = s_targetY = s_targetZ = 0.0f;
    s_baseHeight = 100.0f;
}
void camera_set_pitch(float anguloGraus)
{
    if (anguloGraus > 89.0f)
        anguloGraus = 89.0f;
    if (anguloGraus < -89.0f)
        anguloGraus = -89.0f;
    s_pitch = anguloGraus;
}
void camera_set_target(float x, float y, float z)
{
    s_targetX = x;
    s_targetY = y;
    s_targetZ = z;
}
void camera_set_direcao(float anguloGraus)
{
    s_personagemAngulo = anguloGraus;
}

void camera_set_base_height(float h) { s_baseHeight = h; }
float camera_get_base_height()
{
    return s_baseHeight;
};
void camera_set_mode(CameraMode m) { s_mode = m; }
CameraMode camera_get_mode(void) { return s_mode; }

void camera_cycle_mode(void) { s_mode = (CameraMode)((s_mode + 1) % CAM_COUNT); }

void camera_set_zoom(float delta)
{
    s_distance += delta;
    if (s_distance < 50.0f)
        s_distance = 50.0f;
    if (s_distance > 5000.0f)
        s_distance = 5000.0f;
}

void camera_orbit(float dxPixels, float dyPixels)
{
    s_angleXZ -= dxPixels * 0.5f;
    s_angleXY += dyPixels * 0.5f;
    if (s_angleXY > 89.0f)
        s_angleXY = 89.0f;
    if (s_angleXY < -89.0f)
        s_angleXY = -89.0f;
    while (s_angleXZ >= 360.0f)
        s_angleXZ -= 360.0f;
    while (s_angleXZ < 0.0f)
        s_angleXZ += 360.0f;
}

void camera_reshape(int w, int h)
{
    if (h == 0)
        h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // FOV de 45 graus, plano de corte perto e longe
    gluPerspective(45.0, (double)w / (double)h, 1.0, 10000.0);
    glMatrixMode(GL_MODELVIEW);
}

void camera_apply(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float axz = s_angleXZ * (float)M_PI / 180.0f;
    float axy = s_angleXY * (float)M_PI / 180.0f;

    // Supondo que a arena tenha 150 de altura (Y) e 200 de largura (X)
    // O centro da arena é (0,0,0)

    switch (s_mode)
    {
    case CAM_TOP_DOWN:                  // Tecla 1: Vista de cima
        gluLookAt(0.0, 0.0, s_distance, // Eye: cima no eixo Z
                  0.0, 0.0, 0.0,        // Center: centro da arena
                  0.0, 1.0, 0.0);       // Up: Y do mundo
        break;

    case CAM_FIRST_PERSON:
    {
        float px = s_targetX;
        float py = s_targetY;
        float pz = s_targetZ;

        float yawRad = s_personagemAngulo * (float)M_PI / 180.0f;
        float pitchRad = s_pitch * (float)M_PI / 180.0f;

        // direção do olhar (unitária)
        float dirX = -sinf(yawRad) * cosf(pitchRad);
        float dirY = cosf(yawRad) * cosf(pitchRad);
        float dirZ = sinf(pitchRad);

        float distanciaAtras = 120.0f;
        float alturaCamera = 100.0f;

        float eyeX = px - dirX * distanciaAtras;
        float eyeY = py - dirY * distanciaAtras;
        float eyeZ = pz + alturaCamera + dirZ * distanciaAtras;

        float lookAtX = px + dirX * 100.0f;
        float lookAtY = py + dirY * 100.0f;
        float lookAtZ = pz + alturaCamera + dirZ * 100.0f;

        gluLookAt(eyeX, eyeY, eyeZ,
                  lookAtX, lookAtY, lookAtZ,
                  0.0f, 0.0f, 1.0f);
    }
    break;

    case CAM_ENEMY_VIEW: // Tecla 3: Vista do inimigo para o personagem
        // Inimigo no topo da arena (Y positivo), olhando para baixo (Y negativo)
        {
            float px = s_targetX;         // Pega a posição X do personagem para "seguir"
            gluLookAt(px, 250.0f, 150.0f, // Eye: topo da arena, um pouco elevado
                      px, 0.0f, 0.0f,     // Center: olhando para o centro/baixo
                      0.0, 0.0, 1.0);     // Up: Z para cima
        }
        break;

    case CAM_ARENA_BASE: // Tecla 4: Base olhando para o fundo
    {
        // s_baseHeight regula a altura (Y) da câmera na base
        float h = s_baseHeight;
        float px = s_targetX;          // Segue o personagem no eixo X
        gluLookAt(px, -800.0f, h + 50, // Eye: fundo da arena (Y negativo)
                  px, 300.0f, h,       // Center: olhando para o topo
                  0.0, 0.0, 1.0);      // Up: Z é a altura na tela
    }
    break;

    case CAM_ORBIT: // Extra: Órbita livre
    default:
    {
        float ex = s_targetX + s_distance * sinf(axz) * cosf(axy);
        float ey = s_targetY + s_distance * sinf(axy);
        float ez = s_targetZ + s_distance * cosf(axz) * cosf(axy);
        gluLookAt(ex, ey, ez,
                  s_targetX, s_targetY, s_targetZ,
                  0.0, 1.0, 0.0);
    }
    break;
    }
}