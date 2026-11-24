#include "arena.h"
#include "texture.h"
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

Arena::Arena()
{
    largura = 500.0f;
    profundidade = 150.0f;
    alturaParede = 30.0f;
}

Arena::~Arena()
{
    if (texturaArvore)
        glDeleteTextures(1, &texturaArvore);
    if (texturaChao)
        glDeleteTextures(1, &texturaChao);
    for (int i = 0; i < 6; ++i)
        if (texturasSkybox[i])
            glDeleteTextures(1, &texturasSkybox[i]);
}

void Arena::Configurar(const Configuracao &conf)
{
    if (conf.arena.largura > 0)
        largura = (float)conf.arena.largura;
    if (conf.arena.altura > 0)
        profundidade = (float)conf.arena.altura;
    if (conf.jogador.raioCabeca > 0)
        alturaParede = (float)conf.jogador.raioCabeca;
    else
        alturaParede = 30.0f;
}

void Arena::CarregarTexturas()
{
    if (!texturaArvore)
    {
        texturaArvore = loadBMP("src/assets/arvore.bmp");
        if (texturaArvore == 0)
            fprintf(stderr, "Arena: FALHA ao carregar textura da arvore\n");
        else
            printf("Arena: arvore OK texId=%u\n", texturaArvore);
    }
    if (!texturaChao)
    {
        texturaChao = loadBMP("src/assets/piso2.bmp");
        if (texturaChao == 0)
            fprintf(stderr, "Arena: FALHA ao carregar textura do chao\n");
        else
            printf("Arena: chao OK texId=%u\n", texturaChao);
    }
    CarregarSkybox();
    GaranteMalhas();
}
void Arena::CarregarSkybox()
{
    if (skyboxCarregado)
        return;

    const char *nomes[6] = {
        "src/assets/skybox/right.bmp",
        "src/assets/skybox/left.bmp",
        "src/assets/skybox/top.bmp",
        "src/assets/skybox/bottom.bmp",
        "src/assets/skybox/front.bmp",
        "src/assets/skybox/back.bmp"};

    for (int i = 0; i < 6; ++i)
    {
        texturasSkybox[i] = loadBMP(nomes[i]);
        if (texturasSkybox[i] == 0)
            fprintf(stderr, "Arena: falha ao carregar skybox face %d (%s)\n", i, nomes[i]);
    }
    skyboxCarregado = true;
}
void Arena::DesenhaSkybox() const
{
    if (!skyboxCarregado)
        return;

    float s = 2000.0f;

    GLboolean ilum = glIsEnabled(GL_LIGHTING);
    if (ilum)
        glDisable(GL_LIGHTING);

    glDepthMask(GL_FALSE);
    texture_enable();
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    int rotFace[6] = {
        270, // +X (right)
        90,  // -X (left)
        270, // +Y (top)
        0,   // -Y (baixo)
        0,   // +Z (front)
        180  // -Z (trás)
    };

    auto texUV = [&](int f, float u, float v)
    {
        float du = u - 0.5f;
        float dv = v - 0.5f;
        float su = du, sv = dv;
        switch (rotFace[f])
        {
        case 90:
            su = -dv;
            sv = du;
            break;
        case 180:
            su = -du;
            sv = -dv;
            break;
        case 270:
            su = dv;
            sv = -du;
            break;
        default:
            break; // 0°
        }
        glTexCoord2f(su + 0.5f, sv + 0.5f);
    };

    struct
    {
        int tex;
        float n[3];
        float v[4][3];
    } faces[6] = {
        // +X (right)
        {0, {-1, 0, 0}, {{s, -s, -s}, {s, -s, s}, {s, s, s}, {s, s, -s}}},
        // -X (left)
        {1, {1, 0, 0}, {{-s, -s, s}, {-s, -s, -s}, {-s, s, -s}, {-s, s, s}}},
        // +Y (front)
        {2, {0, 0, -1}, {{-s, -s, s}, {s, -s, s}, {s, s, s}, {-s, s, s}}},
        // -Y (trás)
        {3, {0, 0, 1}, {{s, -s, -s}, {-s, -s, -s}, {-s, s, -s}, {s, s, -s}}},
        // +Z (top)
        {4, {0, -1, 0}, {{-s, s, -s}, {s, s, -s}, {s, s, s}, {-s, s, s}}},
        // -Z (baixo)
        {5, {0, 1, 0}, {{-s, -s, s}, {s, -s, s}, {s, -s, -s}, {-s, -s, -s}}},
    };

    // Coordenadas UV base para os 4 vértices de cada face (ordem dos vértices acima)
    const float uvBase[4][2] = {
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f}};

    for (int f = 0; f < 6; ++f)
    {
        if (texturasSkybox[faces[f].tex] == 0)
            continue;

        glBindTexture(GL_TEXTURE_2D, texturasSkybox[faces[f].tex]);
        glBegin(GL_QUADS);
        glNormal3f(faces[f].n[0], faces[f].n[1], faces[f].n[2]);
        for (int k = 0; k < 4; ++k)
        {
            texUV(f, uvBase[k][0], uvBase[k][1]);
            glVertex3fv(faces[f].v[k]);
        }
        glEnd();
    }

    texture_disable();
    glDepthMask(GL_TRUE);
    if (ilum)
        glEnable(GL_LIGHTING);
}
void Arena::GaranteMalhas()
{
    if (malhasCarregadas)
        return;

    if (!malhaCone.loadMesh("src/assets/cone.obj"))
        fprintf(stderr, "Arena: falha ao carregar cone.obj\n");

    if (!malhaArvore.loadMesh("src/assets/arvore2.obj"))
        fprintf(stderr, "Arena: falha ao carregar arvore.obj\n");

    // ---- Cone ----
    if (!malhaCone.vertsPos.empty())
    {
        float mn[3] = {1e30f, 1e30f, 1e30f};
        float mx[3] = {-1e30f, -1e30f, -1e30f};
        for (auto &v : malhaCone.vertsPos)
        {
            mn[0] = fminf(mn[0], v.x);
            mn[1] = fminf(mn[1], v.y);
            mn[2] = fminf(mn[2], v.z);
            mx[0] = fmaxf(mx[0], v.x);
            mx[1] = fmaxf(mx[1], v.y);
            mx[2] = fmaxf(mx[2], v.z);
        }
        minYMalhaCone = mn[1];
        float altura = mx[1] - mn[1];
        float maior = fmaxf(altura, fmaxf(mx[0] - mn[0], mx[2] - mn[2]));
        escalaCone = (maior > 1e-4f) ? (25.0f / maior) : 1.0f;
    }

    // ---- Árvore ----
    if (!malhaArvore.vertsPos.empty())
    {
        float mn[3] = {1e30f, 1e30f, 1e30f};
        float mx[3] = {-1e30f, -1e30f, -1e30f};
        for (auto &v : malhaArvore.vertsPos)
        {
            mn[0] = fminf(mn[0], v.x);
            mn[1] = fminf(mn[1], v.y);
            mn[2] = fminf(mn[2], v.z);
            mx[0] = fmaxf(mx[0], v.x);
            mx[1] = fmaxf(mx[1], v.y);
            mx[2] = fmaxf(mx[2], v.z);
        }
        minYMalhaArvore = mn[1];
        float altura = mx[1] - mn[1];
        float maior = fmaxf(altura, fmaxf(mx[0] - mn[0], mx[2] - mn[2]));
    }

    malhasCarregadas = true;
}
void Arena::DesenhaCone(float x, float y)
{
    if (malhaCone.vertsPos.empty())
        return;

    glPushMatrix();

    // 1) Posiciona no plano XY
    glTranslatef(x, y, 0.0f);

    // 2) Deita o modelo do Blender (Y-up) para o plano XY do jogo
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    // 3) Escala e apoio no chão
    glScalef(escalaCone, escalaCone, escalaCone);
    glTranslatef(0.0f, -minYMalhaCone, 0.0f);

    // 4) Cor (caso não tenha textura)
    GLfloat matCone[] = {1.0f, 0.5f, 0.0f, 1.0f}; // Laranja
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matCone);

    // 5) Desenha
    malhaCone.draw();

    glPopMatrix();
}

void Arena::DesenhaLinhaDeConesArena() const
{
    if (!malhaCone.vertsPos.empty())
    {
        float cx = -largura / 2;
        float cy = 0;
        for (int i = 0; i < numCones; ++i)
        {
            const_cast<Arena *>(this)->DesenhaCone(cx, cy);
            cx += largura / numCones;
        }
    }
}
void Arena::DesenhaArvore(float x, float y, float angulo)
{

    if (malhaArvore.vertsPos.empty())
        return;

    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    // Modelo Y-up do Blender -> deitar para o plano XY do jogo
    glRotatef(angulo, 0.0f, 0.0f, 1.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    glScalef(escalaArvore, escalaArvore, escalaArvore);
    glTranslatef(0.0f, -minYMalhaArvore, 0.0f);

    bool usaTextura = (texturaArvore != 0);
    if (usaTextura)
    {
        texture_enable();
        glBindTexture(GL_TEXTURE_2D, texturaArvore);
    }

    malhaArvore.draw(usaTextura);

    if (usaTextura)
        texture_disable();

    glPopMatrix();
}
void Arena::DesenhaArvoresParedes() const
{
    if (malhaArvore.vertsPos.empty())
        return;

    float w = largura / 2.0f;
    float d = profundidade / 2.0f;

    Arena *self = const_cast<Arena *>(this);

    // Parede Sul (y = -d) e Norte (y = +d), ao longo de X
    for (int i = 0; i < numArvores / 2; ++i)
    {
        float t = (i + 0.5f) / (numArvores / 2); // 0..1
        float x = -w + t * largura;
        self->DesenhaArvore(x, -d, 90.0);
        self->DesenhaArvore(x, d, 90.0);
    }

    // Parede Oeste (x = -w) e Leste (x = +w), ao longo de Y
    for (int i = 0; i < numArvores; ++i)
    {
        float t = (i + 0.5f) / numArvores;
        float y = -d + t * profundidade;
        self->DesenhaArvore(-w, y, 0);
        self->DesenhaArvore(w, y, 0);
    }
}
void Arena::Desenha() const
{
    float w = largura / 2.0f;
    float d = profundidade / 2.0f;

    // ---- Skybox primeiro (fundo) ----
    DesenhaSkybox();

    // ---- Cones na linha central ----
    DesenhaLinhaDeConesArena();

    // ---- Chão (mesmo piso da arena) ----
    GLfloat matChao[] = {0.2f, 0.6f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matChao);

    if (texturaChao)
    {
        texture_enable();
        glBindTexture(GL_TEXTURE_2D, texturaChao);
        glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-w, -d, 0);
        glTexCoord2f(5.0f, 0.0f);
        glVertex3f(w, -d, 0);
        glTexCoord2f(5.0f, 5.0f);
        glVertex3f(w, d, 0);
        glTexCoord2f(0.0f, 5.0f);
        glVertex3f(-w, d, 0);
        glEnd();
        texture_disable();
    }
    else
    {
        glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glVertex3f(-w, -d, 0);
        glVertex3f(w, -d, 0);
        glVertex3f(w, d, 0);
        glVertex3f(-w, d, 0);
        glEnd();
    }

    DesenhaArvoresParedes();
}