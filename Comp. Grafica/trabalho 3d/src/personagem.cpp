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

// Constrói a matriz que leva um ponto local da arma ao espaço do mundo.
// Deve replicar EXATAMENTE a cadeia do DesenhaArma().
static void constroiMatrizArma(
    float gX, float gY,
    float thetaCorpo,
    float thetaArma, float thetaArmaPitch,
    float malhaEscala, float malhaMinY,
    float maoX, float maoY, float maoZ,
    float armaMinY, float escalaArma,
    mat4 &out)
{
    mat4 t, r, s;

    mat4_identity(out);

    // Translate(gX, gY, 0)
    mat4_translate(t, gX, gY, 0.0f);
    mat4_mul(out, t, out);
    // Rotate(90, X)
    mat4_rotateX(r, 90.0f);
    mat4_mul(out, r, out);
    // Rotate(theta+180, Y)
    mat4_rotateY(r, thetaCorpo + 180.0f);
    mat4_mul(out, r, out);
    // Scale(malhaEscala)
    mat4_scale(s, malhaEscala);
    mat4_mul(out, s, out);
    // Translate(0, -malhaMinY, 0)
    mat4_translate(t, 0.0f, -malhaMinY, 0.0f);
    mat4_mul(out, t, out);
    // Translate(mao)
    mat4_translate(t, maoX, maoY, maoZ);
    mat4_mul(out, t, out);
    // Rotate(thetaArma, Z)
    mat4_rotateZ(r, thetaArma);
    mat4_mul(out, r, out);
    // Rotate(thetaArmaPitch, X)
    mat4_rotateX(r, thetaArmaPitch);
    mat4_mul(out, r, out);
    // Translate(0, -armaMinY, 0)
    mat4_translate(t, 0.0f, -armaMinY, 0.0f);
    mat4_mul(out, t, out);
    // Scale(escalaArma)
    mat4_scale(s, escalaArma);
    mat4_mul(out, s, out);
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
    glRotatef(thetaArma, 0.0f, 0.0f, 1.0f);      // yaw
    glRotatef(thetaArmaPitch, 1.0f, 0.0f, 0.0f); // pitch

    // ---- Corrige pivô da arma + escala ----
    glTranslatef(0.0f, 0.0f, 0.0f); // põe o cabo (minY) na mão
    glScalef(escalaArma, escalaArma, escalaArma);

    // ---- Material ----
    GLfloat matArma[] = {0.25f, 0.25f, 0.28f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matArma);

    malhaArma.draw();
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
        animAndandoFrente.draw();
        break;
    case DIR_TRAS:
        animAndandoTras.draw();
        break;
    case DIR_ESQ:
        animAndandoEsq.draw();
        break;
    case DIR_DIR:
        animAndandoDir.draw();
        break;
    case DIR_NENHUMA:
    default:
        animParado.draw();
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

    if (!malhaArmaCarregada || malhaArma.vertsPos.size() < 85) {
        return new Tiro(gX, gY, zFallback, 0, 1, 0, VelocidadeTiro);
    }

    // Mesmo ponto de ancoragem do DesenhaArma (ombro do personagem)
    const mesh *malhaAtiva = nullptr;
    switch (direcaoAtual) {
        case DIR_FRENTE: malhaAtiva = animAndandoFrente.getCurrentMesh(); break;
        case DIR_TRAS:   malhaAtiva = animAndandoTras.getCurrentMesh();   break;
        case DIR_ESQ:    malhaAtiva = animAndandoEsq.getCurrentMesh();    break;
        case DIR_DIR:    malhaAtiva = animAndandoDir.getCurrentMesh();    break;
        default:         malhaAtiva = animParado.getCurrentMesh();        break;
    }
    if (!malhaAtiva || malhaAtiva->vertsPos.size() < 196) {
        return new Tiro(gX, gY, zFallback, 0, 1, 0, VelocidadeTiro);
    }

    const pos &mao   = malhaAtiva->vertsPos[196 - 1]; // âncora (ombro)
    const pos &ponta = malhaArma.vertsPos[85 - 1];    // ponta do cano

    mat4 M;
    constroiMatrizArma(gX, gY, theta,
                       thetaArma, thetaArmaPitch,
                       malhaEscala, malhaMinY,
                       mao.x, mao.y, mao.z,   // ← âncora correta
                       0.0f, escalaArma, M);

    // Scope (âncora) no mundo = M * (0,0,0)
    float sX, sY, sZ;
    mat4_transformPoint(M, 0, 0, 0, sX, sY, sZ);

    // Ponta no mundo
    float pX, pY, pZ;
    mat4_transformPoint(M, ponta.x, ponta.y, ponta.z, pX, pY, pZ);

    // Direção real
    float dx = pX - sX, dy = pY - sY, dz = pZ - sZ;

    // debug
    m_debugTiroSX = sX; m_debugTiroSY = sY; m_debugTiroSZ = sZ;
    m_debugTiroDX = dx; m_debugTiroDY = dy; m_debugTiroDZ = dz;
    m_debugTiroPX = pX; m_debugTiroPY = pY; m_debugTiroPZ = pZ;
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
}
