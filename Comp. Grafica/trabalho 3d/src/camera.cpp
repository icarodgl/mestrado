#include "camera.h"
#include <GL/glu.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// -------------------------------------------------------------------
// Camera
// -------------------------------------------------------------------
Camera::Camera() { init(); }

void Camera::init()
{
    mode = CAM_FIRST_PERSON;
    distance = 1200.0f;
    angleXZ  = 0.0f;
    angleXY  = 30.0f;
    target   = {0, 0, 0};
    position = {0, 0, 0};
    up       = {0, 0, 1};
    baseHeight = 100.0f;
    pitch = 0.0f;
    personagemAngulo = 0.0f;
    useExplicitPose = false;
}

void Camera::setPitch(float graus)
{
    if (graus >  89.0f) graus =  89.0f;
    if (graus < -89.0f) graus = -89.0f;
    pitch = graus;
}

void Camera::setZoom(float delta)
{
    distance += delta;
    if (distance < 50.0f)   distance = 50.0f;
    if (distance > 5000.0f) distance = 5000.0f;
}

void Camera::orbit(float dxPixels, float dyPixels)
{
    angleXZ -= dxPixels * 0.5f;
    angleXY += dyPixels * 0.5f;
    if (angleXY >  89.0f) angleXY =  89.0f;
    if (angleXY < -89.0f) angleXY = -89.0f;
    while (angleXZ >= 360.0f) angleXZ -= 360.0f;
    while (angleXZ <    0.0f) angleXZ += 360.0f;
}

void Camera::setMode(CameraMode m)
{
    if (m == mode) return;
    mode = m;
    // ao trocar de modo, volta a usar o cálculo interno
    useExplicitPose = false;
}

void Camera::cycleMode()
{
    setMode((CameraMode)((mode + 1) % CAM_COUNT));
}

void Camera::setNearFarForMode()
{
    switch (mode)
    {
    case CAM_FIRST_PERSON:
        nearPlane = 0.5;  farPlane = 5000.0;
        break;
    case CAM_THERD_PERSON:
        nearPlane = 1.0;  farPlane = 5000.0;
        break;
    case CAM_ENEMY_VIEW:
    case CAM_ARENA_BASE:
    case CAM_TOP_DOWN:
    case CAM_ORBIT:
    default:
        nearPlane = 1.0;  farPlane = 10000.0;
        break;
    }
}

Vec3 Camera::computeEye() const    { Vec3 e, l, u; computePose(e, l, u); return e; }
Vec3 Camera::computeLookAt() const { Vec3 e, l, u; computePose(e, l, u); return l; }

// Calcula eye/look/up por modo. Se useExplicitPose==true, retorna
// position/target/up diretamente (controle manual).
void Camera::computePose(Vec3 &eye, Vec3 &look, Vec3 &upv) const
{
    if (useExplicitPose)
    {
        eye  = position;
        look = target;
        upv  = up;
        return;
    }

    const float axz = angleXZ * (float)M_PI / 180.0f;
    const float axy = angleXY * (float)M_PI / 180.0f;
    const float yawRad = personagemAngulo * (float)M_PI / 180.0f;
    const float dirX = -sinf(yawRad);
    const float dirY =  cosf(yawRad);

    switch (mode)
    {
    case CAM_TOP_DOWN:
        eye  = {0, 0, distance};
        look = {0, 0, 0};
        upv  = {0, 1, 0};
        break;

    case CAM_THERD_PERSON:
    {
        float px = target.x;
        float py = target.y;
        float pz = target.z + 80.0f;

        float dist = distance * thirdPersonFactor;
        float alt  = thirdPersonHeight;

        eye  = { px - dirX * dist, py - dirY * dist, pz + alt };
        look = { px, py, pz };
        upv  = { 0, 0, 1 };
        break;
    }

    case CAM_FIRST_PERSON:
    {
        float px = target.x;
        float py = target.y + firstPersonEyeY;
        float pz = target.z + firstPersonEyeZ;

        eye = { px + dirX * firstPersonAdvance,
                py + dirY * firstPersonAdvance,
                pz };

        float pitchRad = pitch * (float)M_PI / 180.0f;
        float cp = cosf(pitchRad), sp = sinf(pitchRad);
        float lookX = dirX * cp;
        float lookY = dirY * cp;
        float lookZ = sp;

        look = { eye.x + lookX, eye.y + lookY, eye.z + lookZ };

        upv = { 0, 0, 1 };
        if (fabsf(sp) > 0.999f) upv = { -dirX, -dirY, 0.0f };
        break;
    }

    case CAM_ENEMY_VIEW:
        eye  = { target.x, 250.0f, 150.0f };
        look = { target.x, 0.0f,   0.0f };
        upv  = { 0, 0, 1 };
        break;

    case CAM_ARENA_BASE:
        eye  = { target.x, 800.0f, baseHeight + 50.0f };
        look = { target.x, 300.0f, baseHeight };
        upv  = { 0, 0, 1 };
        break;

    case CAM_ORBIT:
    default:
        eye  = { target.x + distance * sinf(axz) * cosf(axy),
                 target.y + distance * sinf(axy),
                 target.z + distance * cosf(axz) * cosf(axy) };
        look = { target.x, target.y, target.z };
        upv  = { 0, 1, 0 };
        break;
    }
}

void Camera::reshape(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 10000.0);
    glMatrixMode(GL_MODELVIEW);
    setNearFarForMode();
}

void Camera::apply()
{
    setNearFarForMode();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    gluPerspective(45.0,
                   (double)vp[2] / (double)vp[3],
                   nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Vec3 eye, look, upv;
    computePose(eye, look, upv);

    gluLookAt(eye.x, eye.y, eye.z,
              look.x, look.y, look.z,
              upv.x, upv.y, upv.z);
}

// -------------------------------------------------------------------
// Instância global + wrappers legados
// -------------------------------------------------------------------
static Camera g_camera;

Camera &camera_instance() { return g_camera; }

void camera_init(void)              { g_camera.init(); }
void camera_reshape(int w, int h)   { g_camera.reshape(w, h); }
void camera_apply(void)             { g_camera.apply(); }

void camera_set_mode(CameraMode m)  { g_camera.setMode(m); }
void camera_set_direcao(float a)    { g_camera.setDirection(a); }
CameraMode camera_get_mode(void)    { return g_camera.getMode(); }
void camera_cycle_mode(void)        { g_camera.cycleMode(); }
void camera_set_zoom(float d)       { g_camera.setZoom(d); }
void camera_orbit(float dx, float dy){ g_camera.orbit(dx, dy); }
void camera_set_target(float x, float y, float z) { g_camera.setTarget(x, y, z); }
void camera_set_base_height(float h){ g_camera.setBaseHeight(h); }
float camera_get_base_height()      { return g_camera.getBaseHeight(); }
void camera_set_pitch(float a)      { g_camera.setPitch(a); }
void set_near_plane(float np)       { g_camera.setNearPlane(np); }

void camera_set_position(float x, float y, float z) { g_camera.setPosition(x, y, z); }
void camera_use_explicit_pose(bool on)              { g_camera.useExplicitPose = on; }