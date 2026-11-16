#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "ui.h"
#include "barril.h"
#include "tiro.h"
#include "utils.h"
#include "personagem.h"
#include "configuracao.h"
#include "camera.h"
#include "lighting.h"

Configuracao config;
int keyStatus[256];

GLint Width  = 700;
GLint Height = 700;

// Mouse
int   gMousePixelX = 0, gMousePixelY = 0;
GLfloat gMouseWorldX = 0.0f, gMouseWorldY = 0.0f;
bool  mousePressionado = false;
int   mouseBtnDown = 0;
int   lastMouseX = 0, lastMouseY = 0;

static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);

Personagem personagem;
MyGameUI   gameUI;

static const int qtTiro   = 20;
static const int qtBarril = 6;
GLfloat velocidadeInimigo = 1;
GLfloat tempoCriarBarril  = 0.0;
GLfloat tiroPorSegundo    = 200.0;
GLfloat VelocidadeTiro    = 200.0;
int     ladoBarril        = 100;
GLfloat tempoTiro         = 0.0;
Tiro *tiros[qtTiro]           = {};
Tiro *tirosInimigo[qtTiro]    = {};
Barril *barris[qtBarril]      = {};

// ------------------------------------------------------------------
// Tempo
// ------------------------------------------------------------------
static GLdouble deltaTime()
{
    GLdouble now = glutGet(GLUT_ELAPSED_TIME);
    GLdouble dt  = now - previousTime;
    previousTime = now;
    return dt;
}

// ------------------------------------------------------------------
// Reset
// ------------------------------------------------------------------
static void resetGame()
{
    gameUI.Reset(config);
    for (int i = 0; i < qtTiro; ++i) {
        delete tiros[i];        tiros[i] = nullptr;
        delete tirosInimigo[i]; tirosInimigo[i] = nullptr;
    }
    for (int i = 0; i < qtBarril; ++i) {
        delete barris[i];       barris[i] = nullptr;
    }
}

// ------------------------------------------------------------------
// Tiros
// ------------------------------------------------------------------
static void criaTiros(Tiro *t, Tiro *arr[])
{
    if (!t) return;
    t->Configurar(config);
    for (int i = 0; i < qtTiro; ++i)
        if (arr[i] == nullptr) { arr[i] = t; return; }
    delete t;
}

static void controlaTiros(GLfloat dt)
{
    for (int i = 0; i < qtTiro; ++i) {
        if (tirosInimigo[i]) {
            tirosInimigo[i]->Move(dt);
            if (!tirosInimigo[i]->Valido()) {
                delete tirosInimigo[i]; tirosInimigo[i] = nullptr;
            } else if (tirosInimigo[i]->ColideCom(personagem)) {
                gameUI.Atingido();
                delete tirosInimigo[i]; tirosInimigo[i] = nullptr;
            }
        }
        if (tiros[i]) {
            tiros[i]->Move(dt);
            for (int b = qtBarril - 1; b >= 0; --b) {
                if (barris[b] && tiros[i]->ColideCom(*barris[b])) {
                    barris[b]->Atingido();
                    gameUI.Ponto();
                    if (!barris[b]->Valido()) gameUI.BarrilDestruido();
                    delete tiros[i]; tiros[i] = nullptr;
                    break;
                }
            }
            if (tiros[i] && !tiros[i]->Valido()) {
                delete tiros[i]; tiros[i] = nullptr;
            }
        }
    }
}

static void desenhaTiros()
{
    for (int i = 0; i < qtTiro; ++i) {
        if (tiros[i])        tiros[i]->Desenha();
        if (tirosInimigo[i]) tirosInimigo[i]->Desenha();
    }
}

// ------------------------------------------------------------------
// Barris
// ------------------------------------------------------------------
static void controlaBarris(GLfloat dt)
{
    GLfloat now = glutGet(GLUT_ELAPSED_TIME);
    GLfloat px, py;
    personagem.getPos(px, py);

    if (now > tempoCriarBarril) {
        tempoCriarBarril = now + FloatAleatorio(1000.0, 5000.0);
        GLfloat y = config.arena.altura / 2.0f;
        Barril *b = new Barril(ladoBarril, y, IntAleatorio(0, 1));
        b->Configurar(config);
        for (int i = 0; i < qtBarril; ++i) {
            if (barris[i] == nullptr) {
                ladoBarril *= -1;
                barris[i] = b;
                break;
            }
        }
    }
    for (int i = 0; i < qtBarril; ++i) {
        if (!barris[i]) continue;
        barris[i]->Mirar(px, py);
        criaTiros(barris[i]->Atirar(now, VelocidadeTiro), tirosInimigo);
        barris[i]->Move(dt);

        if (barris[i]->ColideCom(personagem)) {
            gameUI.Atingido();
            delete barris[i]; barris[i] = nullptr;
            continue;
        }
        if (barris[i] && !barris[i]->Valido()) {
            delete barris[i]; barris[i] = nullptr;
        }
    }
}

static void desenhaBarris()
{
    for (int i = 0; i < qtBarril; ++i)
        if (barris[i]) barris[i]->Desenha();
}

// ------------------------------------------------------------------
// HUD 2D (por cima da cena 3D)
// ------------------------------------------------------------------
static void desenhaUI2D()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, Width, Height, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    gameUI.Desenha();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ------------------------------------------------------------------
// Cena
// ------------------------------------------------------------------
void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera_apply();     // 1) câmera
    lighting_apply();   // 2) luzes (na MODELVIEW da câmera)

    // 3) objetos 3D
    personagem.Desenha();
    desenhaTiros();
    desenhaBarris();

    // 4) HUD por cima (troca de projeção e volta)
    desenhaUI2D();

    glutSwapBuffers();
}

// ------------------------------------------------------------------
// Callbacks
// ------------------------------------------------------------------
void keyPress(unsigned char key, int, int)
{
    switch (key) {
    case 'r': case 'R': keyStatus['r'] = 1; break;
    case 'a': case 'A': keyStatus['a'] = 1; break;
    case 'd': case 'D': keyStatus['d'] = 1; break;
    case 'w': case 'W': keyStatus['w'] = 1; break;
    case 's': case 'S': keyStatus['s'] = 1; break;
    case 'q': case 'Q': keyStatus['q'] = 1; break;
    case 'e': case 'E': keyStatus['e'] = 1; break;
    case ' ':           keyStatus[' '] = 1; break;

    case 'c': case 'C': camera_cycle_mode(); break;
    case '+': case '=': camera_set_zoom(-50.0f); break;
    case '-': case '_': camera_set_zoom( 50.0f); break;

    // teclas 2/3/4 trocam preset de iluminação (útil para testar)
    case '2': lighting_preset_jogo();    break;
    case '3': lighting_preset_estudio(); break;
    case '4': lighting_preset_sol();     break;

    case 27: exit(0);
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int, int)
{
    keyStatus[(int)key] = 0;
    glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) mousePressionado = true;
        if (state == GLUT_UP)   mousePressionado = false;
    }
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) { mouseBtnDown = 1; lastMouseX = x; lastMouseY = y; }
        if (state == GLUT_UP)   mouseBtnDown = 0;
    }
}

void mouseAtivo(int x, int y)
{
    if (!mouseBtnDown) return;
    camera_orbit((float)(x - lastMouseX), (float)(y - lastMouseY));
    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

void mousePassivo(int x, int y)
{
    gMousePixelX = x;
    gMousePixelY = y;
    // A mira 3D real virá depois (gluUnProject); por ora só guarda o pixel.
    glutPostRedisplay();
}

// ------------------------------------------------------------------
// Init
// ------------------------------------------------------------------
void init(void)
{
    for (int i = 0; i < 256; ++i) keyStatus[i] = 0;

    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);

    camera_init();
    lighting_init();
    lighting_preset_jogo();

    // Projeção inicial (reshape também fará isso no primeiro frame)
    camera_reshape(Width, Height);
}

void idle(void)
{
    GLfloat dt = deltaTime() / 1000.0f;

    if (keyStatus['a']) personagem.Girar(dt);
    if (keyStatus['d']) personagem.Girar(-dt);
    if (keyStatus['w']) personagem.Mover(dt);
    if (keyStatus['s']) personagem.Mover(-dt);
    if (keyStatus['q']) personagem.MoverLado(-dt);
    if (keyStatus['e']) personagem.MoverLado(dt);

    if (keyStatus[' '] || mousePressionado) {
        GLdouble now = glutGet(GLUT_ELAPSED_TIME);
        if (now > tempoTiro + tiroPorSegundo) {
            tempoTiro = now;
            criaTiros(personagem.Atirar(VelocidadeTiro), tiros);
        }
    }
    if (keyStatus['r']) resetGame();

    personagem.GirarArma(gMouseWorldX, gMouseWorldY);
    controlaTiros(dt);
    controlaBarris(dt);

    glutPostRedisplay();
}

// ------------------------------------------------------------------
// Configuração via XML
// ------------------------------------------------------------------
void ConfigurarUsandoXML()
{
    if (!CarregaConfiguracao("configuracoes.xml", config))
        fprintf(stderr, "Usando valores padrão.\n");

    personagem.Configurar(config);
    gameUI.Configurar(config);

    if (config.tiro.tiroPorSegundo) tiroPorSegundo = 1000.0 / config.tiro.tiroPorSegundo;
    if (config.tiro.velocidade)     VelocidadeTiro = config.tiro.velocidade;
    if (config.barril.velocidade)   velocidadeInimigo = config.barril.velocidade;

    if (config.arena.largura) {
        ladoBarril = config.arena.largura / 4;
        Width      = config.arena.largura;
    }
    if (config.arena.altura) Height = config.arena.altura;
}

// ------------------------------------------------------------------
int main(int argc, char *argv[])
{
    ConfigurarUsandoXML();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150, 50);
    glutCreateWindow("Jogo 3D");

    glutDisplayFunc(renderScene);
    glutReshapeFunc(camera_reshape);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyup);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(mousePassivo);
    glutMotionFunc(mouseAtivo);
    glutMouseFunc(mouseClick);

    init();
    glutMainLoop();
    return 0;
}