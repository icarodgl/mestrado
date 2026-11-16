#include "lighting.h"
#include <string.h>
#include <GL/glut.h>

static Light s_lights[LIGHTING_MAX_LIGHTS];
static int   s_count = 0;
static GLfloat s_globalAmbient[4] = { 0.15f, 0.15f, 0.15f, 1.0f };

static Light light_default()
{
    Light l;
    memset(&l, 0, sizeof(l));
    l.type            = LIGHT_DIRECTIONAL;
    l.ambient[0]=l.ambient[1]=l.ambient[2]=0.10f; l.ambient[3]=1.0f;
    l.diffuse[0]=l.diffuse[1]=l.diffuse[2]=0.90f; l.diffuse[3]=1.0f;
    l.specular[0]=l.specular[1]=l.specular[2]=0.50f; l.specular[3]=1.0f;
    l.position[0]= 300; l.position[1]= 500; l.position[2]= 300; l.position[3]= 0.0f;
    l.attenuation[0]=1.0f; l.attenuation[1]=0.0f; l.attenuation[2]=0.0f;
    l.enabled = true;
    return l;
}

void lighting_init(void)
{
    s_count = 0;
    for (int i = 0; i < LIGHTING_MAX_LIGHTS; ++i) {
        s_lights[i] = light_default();
        s_lights[i].enabled = false;
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, s_globalAmbient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);  // reflexo mais bonito
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}

void lighting_enable_global_ambient(GLfloat r, GLfloat g, GLfloat b)
{
    s_globalAmbient[0]=r; s_globalAmbient[1]=g; s_globalAmbient[2]=b; s_globalAmbient[3]=1.0f;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, s_globalAmbient);
}

int lighting_add(Light l)
{
    if (s_count >= LIGHTING_MAX_LIGHTS) return -1;
    s_lights[s_count] = l;
    return s_count++;
}

void lighting_remove(int idx)
{
    if (idx < 0 || idx >= s_count) return;
    for (int i = idx; i < s_count - 1; ++i) s_lights[i] = s_lights[i+1];
    --s_count;
}

Light* lighting_get(int idx)
{
    if (idx < 0 || idx >= s_count) return nullptr;
    return &s_lights[idx];
}

void lighting_set_enabled(int idx, bool on)
{
    if (idx < 0 || idx >= s_count) return;
    s_lights[idx].enabled = on;
}

int lighting_count(void) { return s_count; }

void lighting_apply(void)
{
    for (int i = 0; i < LIGHTING_MAX_LIGHTS; ++i) {
        GLenum id = GL_LIGHT0 + i;
        if (i < s_count && s_lights[i].enabled) {
            Light& L = s_lights[i];
            // A posição é enviada na MODELVIEW atual (já com gluLookAt aplicado)
            glLightfv(id, GL_POSITION, L.position);
            glLightfv(id, GL_AMBIENT,  L.ambient);
            glLightfv(id, GL_DIFFUSE,  L.diffuse);
            glLightfv(id, GL_SPECULAR, L.specular);
            if (L.type == LIGHT_POINT) {
                glLightf(id, GL_CONSTANT_ATTENUATION,  L.attenuation[0]);
                glLightf(id, GL_LINEAR_ATTENUATION,    L.attenuation[1]);
                glLightf(id, GL_QUADRATIC_ATTENUATION, L.attenuation[2]);
            } else {
                glLightf(id, GL_CONSTANT_ATTENUATION,  1.0f);
                glLightf(id, GL_LINEAR_ATTENUATION,    0.0f);
                glLightf(id, GL_QUADRATIC_ATTENUATION, 0.0f);
            }
            glEnable(id);
        } else {
            glDisable(id);
        }
    }
}

// ------------------------------------------------------------------
// Presets
// ------------------------------------------------------------------

void lighting_preset_jogo(void)
{
    s_count = 0;

    // Luz principal (key) — direcional de cima e um pouco para o lado
    Light key = light_default();
    key.type = LIGHT_DIRECTIONAL;
    key.position[0] = 400; key.position[1] = 800; key.position[2] = 300; key.position[3] = 0;
    key.diffuse[0]=1.0f; key.diffuse[1]=1.0f; key.diffuse[2]=1.0f;
    key.specular[0]=0.3f; key.specular[1]=0.3f; key.specular[2]=0.3f;
    lighting_add(key);

    // Fill — direcional vindo de baixo, cor fria, intensidade baixa
    Light fill = light_default();
    fill.type = LIGHT_DIRECTIONAL;
    fill.position[0] = -300; fill.position[1] = -200; fill.position[2] = -200; fill.position[3] = 0;
    fill.ambient[0]=0.0f; fill.ambient[1]=0.0f; fill.ambient[2]=0.0f;
    fill.diffuse[0]=0.25f; fill.diffuse[1]=0.30f; fill.diffuse[2]=0.45f;
    fill.specular[0]=0.0f; fill.specular[1]=0.0f; fill.specular[2]=0.0f;
    lighting_add(fill);
}

void lighting_preset_estudio(void)
{
    s_count = 0;

    // Key (frente-esquerda)
    Light key = light_default();
    key.position[0]= 500; key.position[1]= 700; key.position[2]= 500; key.position[3]= 1;
    key.diffuse[0]=1.0f; key.diffuse[1]=0.95f; key.diffuse[2]=0.85f;
    key.attenuation[0]=1.0f; key.attenuation[1]=0.0005f; key.attenuation[2]=0.0f;
    key.type = LIGHT_POINT;
    lighting_add(key);

    // Fill (frente-direita, fraca)
    Light fill = light_default();
    fill.position[0]= -500; fill.position[1]= 400; fill.position[2]= 500; fill.position[3]= 1;
    fill.diffuse[0]=0.4f; fill.diffuse[1]=0.45f; fill.diffuse[2]=0.55f;
    fill.attenuation[0]=1.0f; fill.attenuation[1]=0.0005f; fill.attenuation[2]=0.0f;
    fill.type = LIGHT_POINT;
    lighting_add(fill);

    // Rim/Back (atrás, branca forte)
    Light rim = light_default();
    rim.position[0]= 0; rim.position[1]= 500; rim.position[2]= -700; rim.position[3]= 1;
    rim.diffuse[0]=0.9f; rim.diffuse[1]=0.9f; rim.diffuse[2]=1.0f;
    rim.attenuation[0]=1.0f; rim.attenuation[1]=0.0005f; rim.attenuation[2]=0.0f;
    rim.type = LIGHT_POINT;
    lighting_add(rim);
}

void lighting_preset_sol(void)
{
    s_count = 0;
    Light sun = light_default();
    sun.type = LIGHT_DIRECTIONAL;
    sun.position[0]= 0.3f; sun.position[1]= 1.0f; sun.position[2]= 0.2f; sun.position[3]= 0;
    sun.diffuse[0]=1.0f; sun.diffuse[1]=0.98f; sun.diffuse[2]=0.9f;
    sun.ambient[0]=0.2f; sun.ambient[1]=0.2f; sun.ambient[2]=0.25f;
    lighting_add(sun);
}