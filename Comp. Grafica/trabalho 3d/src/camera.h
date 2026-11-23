#ifndef CAMERA_H
#define CAMERA_H

#include <GL/gl.h>

enum CameraMode {
    CAM_TOP_DOWN,
    CAM_FIRST_PERSON,
    CAM_THERD_PERSON,
    CAM_ENEMY_VIEW,
    CAM_ARENA_BASE,
    CAM_ORBIT,
    CAM_COUNT
};

struct Vec3 {
    float x = 0.0f, y = 0.0f, z = 0.0f;
    Vec3() = default;
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

class Camera {
public:
    Camera();

    // ---- estado principal ----
    CameraMode mode = CAM_FIRST_PERSON;

    // posição e alvo "explícitos" (usados quando follow=false)
    Vec3 position;
    Vec3 target;
    Vec3 up = {0.0f, 0.0f, 1.0f};

    // quando true, apply() usa position/target/up direto (sem lógica de modo)
    bool useExplicitPose = false;

    // ---- parâmetros de órbita/zoom ----
    float distance = 1200.0f;
    float angleXZ  = 90.0f;
    float angleXY  = 30.0f;
    float pitch    = 0.0f;   // -89..+89
    float baseHeight = 100.0f;
    float personagemAngulo = 0.0f;

    // offsets por modo (ajuste fino)
    float thirdPersonFactor = 0.15f;
    float thirdPersonHeight = 60.0f;
    float firstPersonEyeZ   = 60.0f;
    float firstPersonAdvance = 15.0f;
    float firstPersonEyeY   = -6.0f;

    // ---- near/far por modo ----
    GLdouble nearPlane = 1.0;
    GLdouble farPlane  = 10000.0;

    // ---- API principal ----
    void init();
    void reshape(int w, int h);
    void apply();                  // aplica projeção + gluLookAt

    void setMode(CameraMode m);
    CameraMode getMode() const { return mode; }
    void cycleMode();

    void setTarget(float x, float y, float z) { target = {x, y, z}; }
    void setTarget(const Vec3 &t) { target = t; }
    Vec3 getTarget() const { return target; }

    void setPosition(float x, float y, float z) { position = {x, y, z}; useExplicitPose = true; }
    void setPosition(const Vec3 &p) { position = p; useExplicitPose = true; }
    Vec3 getPosition() const { return position; }

    void setDirection(float anguloGraus) { personagemAngulo = anguloGraus; }
    float getDirection() const { return personagemAngulo; }

    void setPitch(float graus);
    float getPitch() const { return pitch; }

    void setZoom(float delta);
    void orbit(float dxPixels, float dyPixels);

    void setBaseHeight(float h) { baseHeight = h; }
    float getBaseHeight() const { return baseHeight; }

    void setNearPlane(float np) { nearPlane = np; }
    void setNearFarForMode();

    // helpers que o modo interno usa
    Vec3 computeEye() const;      // só para leitura/debug
    Vec3 computeLookAt() const;

private:
    void computePose(Vec3 &eye, Vec3 &look, Vec3 &upv) const;
};

// ---- API global legada (wrappers) ----
void camera_init(void);
void camera_reshape(int w, int h);
void camera_apply(void);

void camera_set_mode(CameraMode m);
void camera_set_direcao(float anguloGraus);
CameraMode camera_get_mode(void);
void camera_cycle_mode(void);
void camera_set_zoom(float delta);
void camera_orbit(float dxPixels, float dyPixels);
void camera_set_target(float x, float y, float z);
void camera_set_base_height(float h);
float camera_get_base_height();
void camera_set_pitch(float anguloGraus);
void set_near_plane(float np);

// novas (opcionais) — via classe
Camera &camera_instance();
void camera_set_position(float x, float y, float z);
void camera_use_explicit_pose(bool on);

#endif