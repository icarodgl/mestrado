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

static mesh  malhaPersonagem;
static bool  malhaCarregada = false;
static float malhaEscala    = 1.0f;
static float malhaOffsetY   = 0.0f;   // para apoiar a base em y=0 do modelo
static float malhaMinY      = 0.0f;

static void garanteMalhaPersonagem()
{
    if (malhaCarregada) return;

    if (!malhaPersonagem.loadMesh("./src/assets/mirando.obj"))
        fprintf(stderr, "Falha ao carregar assets/mirando.obj\n");

    if (!malhaPersonagem.vertsPos.empty()) {
        float mn[3] = { 1e30f, 1e30f, 1e30f};
        float mx[3] = {-1e30f,-1e30f,-1e30f};
        for (auto& v : malhaPersonagem.vertsPos) {
            mn[0]=fminf(mn[0],v.x); mn[1]=fminf(mn[1],v.y); mn[2]=fminf(mn[2],v.z);
            mx[0]=fmaxf(mx[0],v.x); mx[1]=fmaxf(mx[1],v.y); mx[2]=fmaxf(mx[2],v.z);
        }

        malhaMinY = mn[1];

        // Altura do modelo no Blender (Y-up). Queremos que a altura visual
        // no jogo seja ~ 2 * raioCabeca = 60 unidades (ou ajuste o fator).
        float altura = mx[1] - mn[1];
        float maior  = fmaxf(altura, fmaxf(mx[0]-mn[0], mx[2]-mn[2]));
        malhaEscala = (maior > 1e-4f) ? (90.0f / maior) : 1.0f;
    }
    malhaCarregada = true;
}


void Personagem::GaranteAnimacao()
{
    if (animCarregada) return;

    animParado.load("src/assets/mirando.obj", 0, 0);
    animParado.setFPS(1.0f);
    animParado.setLoop(false);

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
    if (!animCarregada) return;

    // Atualiza todas as animações ativas (só a "atual" precisa de dt,
    // mas resetar as outras evita saltos ao trocar de direção)
    switch (direcaoAtual) {
        case DIR_FRENTE: animAndandoFrente.update(dt); break;
        case DIR_TRAS:   animAndandoTras.update(dt);   break;
        case DIR_ESQ:    animAndandoEsq.update(dt);    break;
        case DIR_DIR:    animAndandoDir.update(dt);    break;
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
    if (animParado.empty() && animAndandoFrente.empty()) return;

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(theta + 180, 0.0f, 1.0f, 0.0f);
    glScalef(malhaEscala, malhaEscala, malhaEscala);
    glTranslatef(0.0f, -malhaMinY, 0.0f);

    switch (direcaoAtual) {
        case DIR_FRENTE: animAndandoFrente.draw(); break;
        case DIR_TRAS:   animAndandoTras.draw();   break;
        case DIR_ESQ:    animAndandoEsq.draw();    break;
        case DIR_DIR:    animAndandoDir.draw();    break;
        case DIR_NENHUMA:
        default:         animParado.draw();        break;
    }

    glPopMatrix();

    SetPosColisor(gX, gY, 0.0);
    DesenhaDebug();
}
void Personagem::GirarArma(int x, int y)
{   
    GLfloat dx = x - gX;
    GLfloat dy = y - gY;
    GLfloat angulo = (atan2f(dy, dx) * 180.0f / M_PI)-90;
    if (angulo > 30 or  angulo < -30)
        return;
    thetaArma = angulo;
}

void Personagem::Mover(GLfloat dir)
{

    GLfloat rad = this->theta * M_PI / 180.0f;
    GLfloat vx = -sinf(rad) * (dir * velocidade);
    GLfloat vy = cosf(rad) * (dir * velocidade);

    
    // arena Altura
    if ((gY > ((0 - raioCabeca)) and vy > 0) or (gY < -((alturaArena/2)-raioCabeca)  and vy < 0))
        return;

    // arena Largura
    if ((gX > ((larguraArena/2)-raioCabeca) and vx > 0) or (gX < -((larguraArena/2) - raioCabeca) and vx < 0))
        return;

    this->gX += vx;
    this->gY += vy;

    SetPosColisor(gX, gY,0.0); // Atualiza o colisor
    
}
void Personagem::MoverLado(GLfloat dir)
{
    if ((gX > larguraArena-raioCabeca and dir > 0) or (gX < -larguraArena+raioCabeca and dir < 0))
        return;
    gX += dir*velocidade;

    SetPosColisor(gX, gY,0.0); // Atualiza o colisor
}
Tiro *Personagem::Atirar(GLfloat VelocidadeTiro)
{

    const GLfloat lx = raioCabeca + (raioCabeca*0.4) - ((raioCabeca*0.4)/2); // braco + centro canhao
    const GLfloat ly = raioCabeca*1.8; // ponta canhao

    const GLfloat DegToRad = M_PI / 180.0f; // Graus pra Radiano
    const GLfloat rad = (thetaArma + theta )* DegToRad;

    const GLfloat c = cosf(rad);
    const GLfloat s = sinf(rad);

    const GLfloat x = gX + lx * c - ly * s;
    const GLfloat y = gY + lx * s + ly * c;

    Tiro *t = new Tiro(x, y, (thetaArma + theta ), VelocidadeTiro);

    return t;
}

bool Personagem::Atingido(Colidivel *obj)
{
    if (!obj)
        return false;
    return ColideCom(*obj);
}

GLfloat Personagem::CalculaBalancoPasso(GLfloat amplitude) const
{
    return sinf(fasePasso) * amplitude *0.06;
}

void Personagem::AtualizaAnimacao(GLfloat dt)  
{
    const GLfloat dx = gX - ultimoX;
    const GLfloat dy = gY - ultimoY;
    const float dist = sqrtf(dx*dx + dy*dy);

    if (dist > 0.01f) {
        emMovimento = true;
        tempoSemMover = 0.0f;
    } else {
        tempoSemMover += dt*1000;
        if (tempoSemMover > 0.1f) emMovimento = false;   // histerese de 100ms
    }

    fasePasso += dist * 0.05f;   // balanço 2D antigo (pode manter para a arma)
    ultimoX = gX;
    ultimoY = gY;
}

void Personagem::SetDirecaoMovimento(int dx, int dy)
{
    if (dx == 0 && dy == 0)      direcaoAtual = DIR_NENHUMA;
    else if (dy > 0)             direcaoAtual = DIR_FRENTE;
    else if (dy < 0)             direcaoAtual = DIR_TRAS;
    else if (dx < 0)             direcaoAtual = DIR_ESQ;
    else if (dx > 0)             direcaoAtual = DIR_DIR;
}