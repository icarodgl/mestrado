#include <math.h>
#include <stdio.h>
#include <vector>
#include "personagem.h"
#include "tiro.h"
#include "utils.h"
#include "objloader.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include "animation.h"

using namespace std;

static mesh malhaPersonagem;
static bool malhaCarregada = false;
static float malhaEscala = 1.0f;
static float malhaOffsetY = 0.0f; // para apoiar a base em y=0 do modelo
static float malhaMinY = 0.0f;
// arma
static mesh malhaArma;
static bool malhaArmaCarregada = false;
static float escalaArma = 1.0;

void Personagem::AtualizaLanterna()
{
    // Se a malha da arma ainda não foi carregada, não há pose válida.
    if (malhaArma.vertsPos.empty())
        return;

    // ---------- 1) Descobre a malha ativa (igual ao Atirar/DesenhaArma) ----------
    const mesh *malhaAtiva = nullptr;
    switch (direcaoAtual) {
        case DIR_FRENTE: malhaAtiva = animAndandoFrente.getCurrentMesh(); break;
        case DIR_TRAS:   malhaAtiva = animAndandoTras.getCurrentMesh();   break;
        case DIR_ESQ:    malhaAtiva = animAndandoEsq.getCurrentMesh();    break;
        case DIR_DIR:    malhaAtiva = animAndandoDir.getCurrentMesh();    break;
        default:         malhaAtiva = animParado.getCurrentMesh();        break;
    }
    if (!malhaAtiva || malhaAtiva->vertsPos.size() < 196)
        return;

    const pos &mao = malhaAtiva->vertsPos[196 - 1];

    // ---------- 2) Empilha a MESMA cadeia do Atirar/DesenhaArma ----------
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(gX, gY, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(theta + 180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(malhaEscala, malhaEscala, malhaEscala);
    glTranslatef(0.0f, -malhaMinY, 0.0f);

    glTranslatef(mao.x, mao.y, mao.z);

    glRotatef(thetaArma, 0.0f, 1.0f, 0.0f);      // yaw
    glRotatef(thetaArmaPitch, 1.0f, 0.0f, 0.0f); // pitch

    glScalef(escalaArma, escalaArma, escalaArma);
    glTranslatef(0.0f, 0.0f, 1.2f);

    GLfloat mv[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);
    glPopMatrix();

    // ---------- 3) Extrai posição e direção do cano ----------
    GLfloat posX = mv[12], posY = mv[13], posZ = mv[14];

    GLfloat dirX = mv[8], dirY = mv[9], dirZ = mv[10];
    float len = sqrtf(dirX*dirX + dirY*dirY + dirZ*dirZ);
    if (len > 1e-6f) { dirX /= len; dirY /= len; dirZ /= len; }
    else             { dirX = 0; dirY = 1; dirZ = 0; }

    // ---------- 4) Configura GL_LIGHT7 como spot do cano ----------
    const GLenum L = GL_LIGHT7;

    GLfloat pos[4]      = { posX, posY, posZ, 1.0f }; // w=1 => spot/point
    GLfloat spotDir[3]  = { dirX, dirY, dirZ };
    GLfloat spotCutoff  = 22.0f;   // abertura do facho (graus)
    GLfloat spotExp     = 12.0f;   // concentração do facho

    GLfloat ambient[4]  = { 0.00f, 0.00f, 0.00f, 1.0f };
    GLfloat diffuse[4]  = { 1.00f, 0.97f, 0.82f, 1.0f }; // tom levemente amarelado
    GLfloat specular[4] = { 1.00f, 1.00f, 1.00f, 1.0f };

    if (lanternaAcesa)
    {
        glEnable(L);

        glLightfv(L, GL_POSITION, pos);
        glLightfv(L, GL_AMBIENT,  ambient);
        glLightfv(L, GL_DIFFUSE,  diffuse);
        glLightfv(L, GL_SPECULAR, specular);

        // atenuação: alcance moderado (ajuste ao seu mundo)
        glLightf(L, GL_CONSTANT_ATTENUATION,  1.0f);
        glLightf(L, GL_LINEAR_ATTENUATION,    0.0025f);
        glLightf(L, GL_QUADRATIC_ATTENUATION, 0.0f);

        // forma de spot
        glLightfv(L, GL_SPOT_DIRECTION, spotDir);
        glLightf (L, GL_SPOT_CUTOFF,    spotCutoff);
        glLightf (L, GL_SPOT_EXPONENT,  spotExp);
    }
    else
    {
        glDisable(L);
    }
}
static void garanteMalhaArma()
{
    if (malhaArmaCarregada)
        return;

    if (!malhaArma.loadMesh("src/assets/arma.obj"))
        fprintf(stderr, "Falha ao carregar arma.obj\n");

    if (!malhaArma.vertsPos.empty())
    {
        float mn[3] = {1e30f, 1e30f, 1e30f};
        float mx[3] = {-1e30f, -1e30f, -1e30f};
        for (auto &v : malhaArma.vertsPos)
        {
            mn[0] = fminf(mn[0], v.x);
            mn[1] = fminf(mn[1], v.y);
            mn[2] = fminf(mn[2], v.z);
            mx[0] = fmaxf(mx[0], v.x);
            mx[1] = fmaxf(mx[1], v.y);
            mx[2] = fmaxf(mx[2], v.z);
        }
    }
    malhaArmaCarregada = true;
}
void Personagem::DesenhaArma()
{
    garanteMalhaArma();
    if (malhaArma.vertsPos.empty())
        return;

    // ---- Malha ativa da animação ----
    const mesh *malhaAtiva = nullptr;
    switch (direcaoAtual)
    {
    case DIR_FRENTE:
        malhaAtiva = animAndandoFrente.getCurrentMesh();
        break;
    case DIR_TRAS:
        malhaAtiva = animAndandoTras.getCurrentMesh();
        break;
    case DIR_ESQ:
        malhaAtiva = animAndandoEsq.getCurrentMesh();
        break;
    case DIR_DIR:
        malhaAtiva = animAndandoDir.getCurrentMesh();
        break;
    default:
        malhaAtiva = animParado.getCurrentMesh();
        break;
    }
    if (!malhaAtiva || malhaAtiva->vertsPos.size() < 100)
        return;

    // ---- Vértice 100 = posição da mão (1-based no .obj) ----
    // ombro  face# 1014 : vert# (196 201 202)

    const pos &mao = malhaAtiva->vertsPos[196 - 1];

    // ---- Mesma transformação do corpo ----
    glPushMatrix();

    glTranslatef(gX, gY, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);       // deita Blender Y-up
    glRotatef(theta + 180, 0.0f, 1.0f, 0.0f); // gira com o corpo
    glScalef(malhaEscala, malhaEscala, malhaEscala);
    glTranslatef(0.0f, -malhaMinY, 0.0f);

    // ---- Ancora no vértice 100 (coordenadas Blender) ----
    glTranslatef(mao.x, mao.y, mao.z);

    // ---- Mira do mouse ----
    glRotatef(thetaArma, 0.0f, 1.0f, 0.0f);      // yaw
    glRotatef(thetaArmaPitch, 1.0f, 0.0f, 0.0f); // pitch

    // ---- Corrige pivô da arma + escala ----
    glTranslatef(0.0f, 0.0f, 0.0f); // põe o cabo (minY) na mão
    glScalef(escalaArma, escalaArma, escalaArma);

    // ---- Material ----
    GLfloat matArma[] = {0.25f, 0.25f, 0.28f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matArma);

    malhaArma.draw(0);
    glPopMatrix();
}
static void garanteMalhaPersonagem()
{
    if (malhaCarregada)
        return;

    if (!malhaPersonagem.loadMesh("./src/assets/mirando.obj"))
        fprintf(stderr, "Falha ao carregar assets/mirando.obj\n");

    if (!malhaPersonagem.vertsPos.empty())
    {
        float mn[3] = {1e30f, 1e30f, 1e30f};
        float mx[3] = {-1e30f, -1e30f, -1e30f};
        for (auto &v : malhaPersonagem.vertsPos)
        {
            mn[0] = fminf(mn[0], v.x);
            mn[1] = fminf(mn[1], v.y);
            mn[2] = fminf(mn[2], v.z);
            mx[0] = fmaxf(mx[0], v.x);
            mx[1] = fmaxf(mx[1], v.y);
            mx[2] = fmaxf(mx[2], v.z);
        }

        malhaMinY = mn[1];

        // Altura do modelo no Blender (Y-up). Queremos que a altura visual
        // no jogo seja ~ 2 * raioCabeca = 60 unidades (ou ajuste o fator).
        float altura = mx[1] - mn[1];
        float maior = fmaxf(altura, fmaxf(mx[0] - mn[0], mx[2] - mn[2]));
        malhaEscala = (maior > 1e-4f) ? (90.0f / maior) : 1.0f;
    }
    malhaCarregada = true;
}

void Personagem::GaranteAnimacao()
{
    if (animCarregada)
        return;

    animParado.load("src/assets/mirando.obj", 0, 0);
    animParado.setFPS(1.0f);
    animParado.setLoop(true);

    animAndandoFrente.load("src/assets/andando_frente/passo%d.obj", 1, 11);
    animAndandoFrente.setFPS(12.0f);
    animAndandoFrente.setLoop(true);

    animAndandoTras.load("src/assets/andando_tras/passo%d.obj", 1, 11);
    animAndandoTras.setFPS(12.0f);
    animAndandoTras.setLoop(true);

    animAndandoEsq.load("src/assets/andando_esq/passo%d.obj", 1, 5);
    animAndandoEsq.setFPS(12.0f);
    animAndandoEsq.setLoop(true);

    animAndandoDir.load("src/assets/andando_dir/passo%d.obj", 1, 5);
    animAndandoDir.setFPS(12.0f);
    animAndandoDir.setLoop(true);

    animCarregada = true;
}
void Personagem::AtualizaAnimacaoAnim(float dt)
{
    if (!animCarregada)
        return;

    // Atualiza todas as animações ativas (só a "atual" precisa de dt,
    // mas resetar as outras evita saltos ao trocar de direção)
    switch (direcaoAtual)
    {
    case DIR_FRENTE:
        animAndandoFrente.update(dt);
        break;
    case DIR_TRAS:
        animAndandoTras.update(dt);
        break;
    case DIR_ESQ:
        animAndandoEsq.update(dt);
        break;
    case DIR_DIR:
        animAndandoDir.update(dt);
        break;
    case DIR_NENHUMA:
    default:
        animParado.update(dt);
        // reseta as outras para começarem do frame 0 ao retomar
        animAndandoFrente.reset();
        animAndandoTras.reset();
        animAndandoEsq.reset();
        animAndandoDir.reset();
        break;
    }

    emMovimento = (direcaoAtual != DIR_NENHUMA);

    // guarda posição para o próximo frame (não mais usado para decidir animação)
    ultimoX = gX;
    ultimoY = gY;
}
void Personagem::DesenhaPersonagem(GLfloat x, GLfloat y)
{
    garanteMalhaPersonagem();

    // Verifica se há animação carregada
    if (animParado.empty() && animAndandoFrente.empty())
        return;

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(theta + 180, 0.0f, 1.0f, 0.0f);
    glScalef(malhaEscala, malhaEscala, malhaEscala);
    glTranslatef(0.0f, -malhaMinY, 0.0f);

    switch (direcaoAtual)
    {
    case DIR_FRENTE:
        animAndandoFrente.draw(0);
        break;
    case DIR_TRAS:
        animAndandoTras.draw(0);
        break;
    case DIR_ESQ:
        animAndandoEsq.draw(0);
        break;
    case DIR_DIR:
        animAndandoDir.draw(0);
        break;
    case DIR_NENHUMA:
    default:
        animParado.draw(0);
        break;
    }

    glPopMatrix();

    // SetPosColisor(gX, gY, 0.0);
}
void Personagem::GirarArma(int x, int y)
{
    GLfloat dx = x - gX;
    GLfloat dy = y - gY;
    GLfloat angulo = (atan2f(dy, dx) * 180.0f / M_PI) - 90;
    if (angulo > 30 or angulo < -30)
        return;
    thetaArma = angulo;
}

void Personagem::Mover(GLfloat dir)
{

    GLfloat rad = this->theta * M_PI / 180.0f;
    GLfloat vx = -sinf(rad) * (dir * velocidade);
    GLfloat vy = cosf(rad) * (dir * velocidade);

    // arena Altura
    if ((gY > ((0 - raioCabeca)) and vy > 0) or (gY < -((alturaArena / 2) - raioCabeca) and vy < 0))
        return;

    // arena Largura
    if ((gX > ((larguraArena / 2) - raioCabeca) and vx > 0) or (gX < -((larguraArena / 2) - raioCabeca) and vx < 0))
        return;

    this->gX += vx;
    this->gY += vy;

    SetPosColisor(gX, gY, 0.0); // Atualiza o colisor
}
void Personagem::MoverLado(GLfloat dir)
{
    if ((gX > larguraArena - raioCabeca and dir > 0) or (gX < -larguraArena + raioCabeca and dir < 0))
        return;
    gX += dir * velocidade;

    SetPosColisor(gX, gY, 0.0); // Atualiza o colisor
}
Tiro *Personagem::Atirar(GLfloat VelocidadeTiro)
{
    const float zFallback = 50.0f;

    if (!malhaArmaCarregada || malhaArma.vertsPos.empty())
        return new Tiro(gX, gY, zFallback, 0, 1, 0, VelocidadeTiro);

    // malha ativa (igual ao DesenhaArma)
    const mesh *malhaAtiva = nullptr;
    switch (direcaoAtual) {
        case DIR_FRENTE: malhaAtiva = animAndandoFrente.getCurrentMesh(); break;
        case DIR_TRAS:   malhaAtiva = animAndandoTras.getCurrentMesh();   break;
        case DIR_ESQ:    malhaAtiva = animAndandoEsq.getCurrentMesh();    break;
        case DIR_DIR:    malhaAtiva = animAndandoDir.getCurrentMesh();    break;
        default:         malhaAtiva = animParado.getCurrentMesh();        break;
    }
    if (!malhaAtiva || malhaAtiva->vertsPos.size() < 196)
        return new Tiro(gX, gY, zFallback, 0, 1, 0, VelocidadeTiro);

    const pos &mao = malhaAtiva->vertsPos[196 - 1];  // só para a cadeia

    // ===== MESMA cadeia de DesenhaArma, lida no final =====
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(gX, gY, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(theta + 180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(malhaEscala, malhaEscala, malhaEscala);
    glTranslatef(0.0f, -malhaMinY, 0.0f);

    // ---- pivô da ARMA (não do tiro) ----
    glTranslatef(mao.x, mao.y, mao.z);

    glRotatef(thetaArma, 0.0f, 1.0f, 0.0f);      // yaw
    glRotatef(thetaArmaPitch, 1.0f, 0.0f, 0.0f); // pitch

    // se quiser colocar o pivô da arma no minY do OBJ da arma:
    // glTranslatef(0.0f, -armaMinY, 0.0f);

    glScalef(escalaArma, escalaArma, escalaArma);
    glTranslatef(0.0f, 0.05f, 1.1f);

    GLfloat mv[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);
    glPopMatrix();

    // origem = pivô da arma no mundo
    float sX = mv[12], sY = mv[13], sZ = mv[14];

    // // direção = eixo do cano da arma (coluna Y da MODELVIEW)
    // float dx = mv[4], dy = mv[5], dz = mv[6];

        // direção = eixo do cano da arma (coluna Z da MODELVIEW, pois o OBJ aponta em +Z)
    float dx = mv[8], dy = mv[9], dz = mv[10];
    {
        float len = sqrtf(dx*dx + dy*dy + dz*dz);
        if (len < 1e-6f) { dx = 0; dy = 1; dz = 0; len = 1.0f; }
        dx /= len; dy /= len; dz /= len;
    }


    m_debugTiroSX = sX; m_debugTiroSY = sY; m_debugTiroSZ = sZ;
    m_debugTiroDX = dx; m_debugTiroDY = dy; m_debugTiroDZ = dz;
    m_debugTiroPX = sX + dx; m_debugTiroPY = sY + dy; m_debugTiroPZ = sZ + dz;
    m_debugTiroValido = true;

    return new Tiro(sX, sY, sZ, dx, dy, dz, VelocidadeTiro);
}

bool Personagem::Atingido(Colidivel *obj)
{
    if (!obj)
        return false;
    return ColideCom(*obj);
}

GLfloat Personagem::CalculaBalancoPasso(GLfloat amplitude) const
{
    return sinf(fasePasso) * amplitude * 0.06;
}

void Personagem::AtualizaAnimacao(GLfloat dt)
{
    const GLfloat dx = gX - ultimoX;
    const GLfloat dy = gY - ultimoY;
    const float dist = sqrtf(dx * dx + dy * dy);

    if (dist > 0.01f)
    {
        emMovimento = true;
        tempoSemMover = 0.0f;
    }
    else
    {
        tempoSemMover += dt * 1000;
        if (tempoSemMover > 0.1f)
            emMovimento = false; // histerese de 100ms
    }

    fasePasso += dist * 0.05f; // balanço 2D antigo (pode manter para a arma)
    ultimoX = gX;
    ultimoY = gY;
}

void Personagem::SetDirecaoMovimento(int dx, int dy)
{
    if (dx == 0 && dy == 0)
        direcaoAtual = DIR_NENHUMA;
    else if (dy > 0)
        direcaoAtual = DIR_FRENTE;
    else if (dy < 0)
        direcaoAtual = DIR_TRAS;
    else if (dx < 0)
        direcaoAtual = DIR_ESQ;
    else if (dx > 0)
        direcaoAtual = DIR_DIR;
}

void Personagem::Desenha()
{
    DesenhaPersonagem(gX, gY);
    DesenhaArma();
    DesenhaDebug();
    desenhaDebugTiro();
    AtualizaLanterna();
}

void Personagem::desenhaDebugTiro()
{
    if (!debugMode)
        return;

    float SX, SY, SZ, DX, DY, DZ, PX, PY, PZ;
    getdebugTiro(SX, SY, SZ, DX, DY, DZ, PX, PY, PZ);

    norm3(DX, DY, DZ);

    // base ortonormal perpendicular a D
    float UX, UY, UZ, VX, VY, VZ;
    basePerp(DX, DY, DZ, UX, UY, UZ, VX, VY, VZ);

    const float halfGraus = 30.0f;               // 60° totais
    const float halfRad   = halfGraus * (float)M_PI / 180.0f;
    const float tanHalf   = tanf(halfRad);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glLineWidth(2.0f);

    // ---- eixo do cone (linha amarela) ----
    glColor3f(1, 1, 0);
    glBegin(GL_LINES);
        glVertex3f(SX, SY, SZ);
        glVertex3f(SX + DX * 200, SY + DY * 200, SZ + DZ * 200);
    glEnd();

    // ---- três "fatias" de cone em comprimentos crescentes ----
    // desenha círculo base + geratrizes em cada fatia
    const int SEG   = 32;
    const int FATIAS = 3;
    for (int f = 1; f <= FATIAS; ++f)
    {
        float L    = 60.0f * f;          // distância ao longo do eixo D
        float raio = L * tanHalf;        // raio do círculo nessa fatia

        // centro da fatia
        float CX = SX + DX * L;
        float CY = SY + DY * L;
        float CZ = SZ + DZ * L;

        // ---- círculo ----
        glColor3f(1.0f, 0.6f, 0.0f);     // laranja
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < SEG; ++i)
        {
            float a = (float)i / SEG * 2.0f * (float)M_PI;
            float ca = cosf(a), sa = sinf(a);
            float x = CX + raio * (ca * UX + sa * VX);
            float y = CY + raio * (ca * UY + sa * VY);
            float z = CZ + raio * (ca * UZ + sa * VZ);
            glVertex3f(x, y, z);
        }
        glEnd();

        // ---- geratrizes (a cada 90° => 4 linhas saindo do ápice) ----
        glColor3f(0.6f, 0.6f, 0.0f);     // amarelo escuro
        glBegin(GL_LINES);
        for (int k = 0; k < 4; ++k)
        {
            float a = k * (float)M_PI / 2.0f;
            float ca = cosf(a), sa = sinf(a);
            float x = CX + raio * (ca * UX + sa * VX);
            float y = CY + raio * (ca * UY + sa * VY);
            float z = CZ + raio * (ca * UZ + sa * VZ);
            glVertex3f(SX, SY, SZ);
            glVertex3f(x, y, z);
        }
        glEnd();
    }

    // ---- ponto de origem (vermelho) ----
    glPointSize(10);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
        glVertex3f(SX, SY, SZ);
    glEnd();

    // ---- ponta calculada / direção real (azul) ----
    glColor3f(0.2f, 0.5f, 1.0f);
    glBegin(GL_POINTS);
        glVertex3f(PX, PY, PZ);
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}
// personagem.cpp
bool Personagem::GetPoseCano(float &px, float &py, float &pz,
                             float &dx, float &dy, float &dz) const
{
    if (malhaArma.vertsPos.empty())
        return false;

    const mesh *malhaAtiva = nullptr;
    switch (direcaoAtual) {
        case DIR_FRENTE: malhaAtiva = animAndandoFrente.getCurrentMesh(); break;
        case DIR_TRAS:   malhaAtiva = animAndandoTras.getCurrentMesh();   break;
        case DIR_ESQ:    malhaAtiva = animAndandoEsq.getCurrentMesh();    break;
        case DIR_DIR:    malhaAtiva = animAndandoDir.getCurrentMesh();    break;
        default:         malhaAtiva = animParado.getCurrentMesh();        break;
    }
    if (!malhaAtiva || malhaAtiva->vertsPos.size() < 196)
        return false;

    const pos &mao = malhaAtiva->vertsPos[196 - 1];

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(gX, gY, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(theta + 180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(malhaEscala, malhaEscala, malhaEscala);
    glTranslatef(0.0f, -malhaMinY, 0.0f);

    glTranslatef(mao.x, mao.y, mao.z);

    glRotatef(thetaArma,      0.0f, 1.0f, 0.0f);
    glRotatef(thetaArmaPitch, 1.0f, 0.0f, 0.0f);

    glScalef(escalaArma, escalaArma, escalaArma);
    glTranslatef(0.0f, 0.2f, 0.8f);

    GLfloat mv[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, mv);
    glPopMatrix();

    px = mv[12]; py = mv[13]; pz = mv[14];
    dx = mv[8];  dy = mv[9];  dz = mv[10];

    float len = sqrtf(dx*dx + dy*dy + dz*dz);
    if (len < 1e-6f) { dx = 0; dy = 1; dz = 0; }
    else { dx /= len; dy /= len; dz /= len; }
    return true;
}
