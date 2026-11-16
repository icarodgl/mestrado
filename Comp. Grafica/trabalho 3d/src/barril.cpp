#include "barril.h"
#include "objloader.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

static mesh  malhaBarril;
static bool  malhaCarregada = false;

static void garanteMalhaBarril()
{
    if (malhaCarregada) return;

    if (!malhaBarril.loadMesh("assets/barrel.obj"))
        fprintf(stderr, "Falha ao carregar assets/barrel.obj\n");

    // ---- Opcional: imprime bbox para você calibrar escala/origem ----
    if (!malhaBarril.vertsPos.empty()) {
        float mn[3] = { 1e30f, 1e30f, 1e30f};
        float mx[3] = {-1e30f,-1e30f,-1e30f};
        for (auto& v : malhaBarril.vertsPos) {
            mn[0]=fminf(mn[0],v.x); mn[1]=fminf(mn[1],v.y); mn[2]=fminf(mn[2],v.z);
            mx[0]=fmaxf(mx[0],v.x); mx[1]=fmaxf(mx[1],v.y); mx[2]=fmaxf(mx[2],v.z);
        }
        printf("barrel.obj bbox: X[%.3f..%.3f] Y[%.3f..%.3f] Z[%.3f..%.3f]\n",
               mn[0],mx[0], mn[1],mx[1], mn[2],mx[2]);
    }
    malhaCarregada = true;
}
void Barril::Desenha()
{
    garanteMalhaBarril();

    glPushMatrix();

    // Posição da entidade no plano do jogo (x, y) — z = 0
    glTranslatef(gX, gY, 0.0f);

    // O Blender exporta Y-up. Para "deitar" o modelo no plano XY do jogo
    // (de pé, olhando para +Z), gire -90° em X.
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    // Escala: ajuste após ver o bbox no console.
    // Se o bbox for, digamos, ~2 unidades de altura e você quer um barril
    // de ~60 unidades no jogo, use escala = 30.
    const float escala = 30.0f;
    glScalef(escala, escala, escala);

    // Rotação própria da entidade (opcional)
    // glRotatef(anguloGraus, 0, 0, 1);

    malhaBarril.draw();

    glPopMatrix();
}
Tiro *Barril::Atirar(GLfloat deltaTime, GLfloat velocidadeTiro)
{
    if (inimigo and deltaTime > disparo +( 1000.0 / tiroPorSegundo))
    {
        disparo = deltaTime;

        const GLfloat lx = raioCabeca + (raioCabeca*0.4) - ((raioCabeca*0.4)/2); // braco + centro canhao
        const GLfloat ly = raioCabeca*1.8; // ponta canhao

        const GLfloat DegToRad = M_PI / 180.0f; // Graus pra Radiano
        const GLfloat rad = (theta) * DegToRad;

        const GLfloat c = cosf(rad);
        const GLfloat s = sinf(rad);

        const GLfloat x = (gX + (width / 2)) + lx * c - ly * s;
        const GLfloat y = (gY + (height / 2)) + lx * s + ly * c;

        Tiro *t = new Tiro(x, y, theta, velocidadeTiro);

        return t;
    }
    return nullptr;
}
