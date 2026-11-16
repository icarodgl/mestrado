#ifndef LIGHTING_H
#define LIGHTING_H

#include <GL/gl.h>

// Máximo de luzes simultâneas (GL_LIGHT0..GL_LIGHT7 = 8)
#define LIGHTING_MAX_LIGHTS 8

// Tipos de luz
enum LightType {
    LIGHT_DIRECTIONAL,   // direcional (como o sol)
    LIGHT_POINT          // pontual com atenuação
};

struct Light {
    LightType type;
    GLfloat   ambient[4];
    GLfloat   diffuse[4];
    GLfloat   specular[4];
    GLfloat   position[4];   // (x,y,z,w) — w=0 direcional, w=1 pontual
    GLfloat   spotDirection[3];   // ignorado por enquanto (sem spot)
    GLfloat   attenuation[3];     // (constante, linear, quadrática) — só pontual
    GLfloat   spotCutoff;         // ignorado por enquanto
    bool      enabled;
};

void lighting_init(void);                         // zera contadores, luz default
void lighting_apply(void);                        // aplica todas as luzes no estado GL
void lighting_enable_global_ambient(GLfloat r, GLfloat g, GLfloat b);

// Gerenciamento de luzes
int  lighting_add(Light l);                       // retorna índice, -1 se cheio
void lighting_remove(int idx);
Light* lighting_get(int idx);                     // nullptr se inválido
void lighting_set_enabled(int idx, bool on);
int  lighting_count(void);

// Presets prontos (substituem todas as luzes)
void lighting_preset_jogo(void);                  // luz principal + fill
void lighting_preset_estudio(void);               // 3-point (key/fill/rim)
void lighting_preset_sol(void);                   // direcional dura

#endif