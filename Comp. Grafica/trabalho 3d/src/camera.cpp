#include "camera.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ---- Estado da câmera ----
static CameraMode s_mode       = CAM_ORBIT;
static float      s_distance   = 700.0f;
static float      s_angleXZ    = 90.0f;   // azimute (graus)
static float      s_angleXY    = 30.0f;   // elevação (graus)
static float      s_targetX    = 0.0f;
static float      s_targetY    = 0.0f;
static float      s_targetZ    = 0.0f;
static int        s_winW       = 700;
static int        s_winH       = 700;

void camera_init(void)
{
    s_mode     = CAM_ORBIT;
    s_distance = 1700.0f;
    s_angleXZ  = 90.0f;
    s_angleXY  = 30.0f;
    s_targetX  = s_targetY = s_targetZ = 0.0f;
}

void camera_set_target(float x, float y, float z)
{
    s_targetX = x; s_targetY = y; s_targetZ = z;
}

void camera_set_mode(CameraMode m) { s_mode = m; }
CameraMode camera_get_mode(void)   { return s_mode; }

void camera_cycle_mode(void)
{
    s_mode = (CameraMode)((s_mode + 1) % CAM_COUNT);
}

void camera_set_zoom(float delta)
{
    s_distance += delta;
    if (s_distance < 50.0f)   s_distance = 50.0f;
    if (s_distance > 5000.0f) s_distance = 5000.0f;
}

void camera_orbit(float dxPixels, float dyPixels)
{
    s_angleXZ -= dxPixels * 0.5f;
    s_angleXY += dyPixels * 0.5f;

    // Evita a câmera "virar do avesso" no polo
    if (s_angleXY >  89.0f) s_angleXY =  89.0f;
    if (s_angleXY < -89.0f) s_angleXY = -89.0f;

    // Mantém o azimute em [0,360) para estabilidade numérica
    while (s_angleXZ >= 360.0f) s_angleXZ -= 360.0f;
    while (s_angleXZ <    0.0f) s_angleXZ += 360.0f;
}

void camera_reshape(int w, int h)
{
    if (h == 0) h = 1;
    s_winW = w;
    s_winH = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Todas as câmeras usam perspectiva; mudam só o "eye/lookAt"
    gluPerspective(45.0, (double)w / (double)h, 1.0, 10000.0);

    glMatrixMode(GL_MODELVIEW);
}

void camera_apply(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float axz = s_angleXZ * (float)M_PI / 180.0f;
    float axy = s_angleXY * (float)M_PI / 180.0f;

    switch (s_mode) {
    case CAM_ORBIT:
    case CAM_FIXED:
    {
        float ex = s_targetX + s_distance * sinf(axz) * cosf(axy);
        float ey = s_targetY + s_distance * sinf(axy);
        float ez = s_targetZ + s_distance * cosf(axz) * cosf(axy);

        // up (0,1,0) — o mundo do jogo usa Y como "cima"
        gluLookAt(ex, ey, ez,
                  s_targetX, s_targetY, s_targetZ,
                  0.0, 1.0, 0.0);
        break;
    }
    case CAM_TOP_DOWN:
    {
        float ex = s_targetX;
        float ey = s_targetY + s_distance;
        float ez = s_targetZ + 0.001f;   // evita degeneração do up com lookAt

        gluLookAt(ex, ey, ez,
                  s_targetX, s_targetY, s_targetZ,
                  0.0, 0.0, -1.0);
        break;
    }
    default: break;
    }
}