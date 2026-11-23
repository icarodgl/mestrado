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
#include "arena.h"

Configuracao config;
int keyStatus[256];

GLint Width = 1200;
GLint Height = 900;

// Mouse
static GLdouble s_modelview[16];
static GLdouble s_projection[16];
static GLint s_viewport[4];

int gMousePixelX = 0, gMousePixelY = 0;
GLfloat gMouseWorldX = 0.0f, gMouseWorldY = 0.0f;
bool mousePressionado = false;
int mouseBtnDown = 0;
int lastMouseX = 0, lastMouseY = 0;

static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);

bool debugColisor = false;
bool pause = false;

Personagem personagem;
MyGameUI gameUI;
Arena arena;
static const int qtTiro = 20;
static const int qtBarril = 6;
GLfloat velocidadeInimigo = 1;
GLfloat tempoCriarBarril = 0.0;
GLfloat tiroPorSegundo = 200.0;
GLfloat VelocidadeTiro = 200.0;
int ladoBarril = 100;
GLfloat tempoTiro = 0.0;
Tiro *tiros[qtTiro] = {};
Tiro *tirosInimigo[qtTiro] = {};
Barril *barris[qtBarril] = {};
int InimigoCamera = -1;

// ------------------------------------------------------------------
// Tempo
// ------------------------------------------------------------------
static GLdouble deltaTime()
{
    GLdouble now = glutGet(GLUT_ELAPSED_TIME);
    GLdouble dt = now - previousTime;
    previousTime = now;
    return dt;
}
// ------------------------------------------------------------------
// Mirando com mouse
static void capturaMatrizesCamera()
{
    glGetDoublev(GL_MODELVIEW_MATRIX, s_modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, s_projection);
    glGetIntegerv(GL_VIEWPORT, s_viewport);
}
// Converte um pixel (x,y) num raio 3D no mundo.
// Retorna origem (no plano near) e direção normalizada.
static bool pixelParaRaio(int mx, int my,
                          float &ox, float &oy, float &oz,
                          float &dx, float &dy, float &dz)
{
    // GLUT tem Y invertido em relação ao OpenGL
    GLdouble nearX, nearY, nearZ, farX, farY, farZ;
    if (!gluUnProject(mx, s_viewport[3] - my, 0.0,
                      s_modelview, s_projection, s_viewport,
                      &nearX, &nearY, &nearZ))
        return false;
    if (!gluUnProject(mx, s_viewport[3] - my, 1.0,
                      s_modelview, s_projection, s_viewport,
                      &farX, &farY, &farZ))
        return false;

    ox = (float)nearX;
    oy = (float)nearY;
    oz = (float)nearZ;
    dx = (float)(farX - nearX);
    dy = (float)(farY - nearY);
    dz = (float)(farZ - nearZ);
    float len = sqrtf(dx * dx + dy * dy + dz * dz);
    if (len < 1e-6f)
        return false;
    dx /= len;
    dy /= len;
    dz /= len;
    return true;
}
// ------------------------------------------------------------------
// Reset
// ------------------------------------------------------------------
static void resetGame()
{
    gameUI.Reset(config);
    for (int i = 0; i < qtTiro; ++i)
    {
        delete tiros[i];
        tiros[i] = nullptr;
        delete tirosInimigo[i];
        tirosInimigo[i] = nullptr;
    }
    for (int i = 0; i < qtBarril; ++i)
    {
        delete barris[i];
        barris[i] = nullptr;
    }
}

// ------------------------------------------------------------------
// Tiros
// ------------------------------------------------------------------
static void criaTiros(Tiro *t, Tiro *arr[])
{
    if (!t)
        return;
    t->Configurar(config);
    for (int i = 0; i < qtTiro; ++i)
        if (arr[i] == nullptr)
        {
            arr[i] = t;
            return;
        }
    delete t;
}

static void controlaTiros(GLfloat dt)
{
    if (pause)
        return;

    for (int i = 0; i < qtTiro; ++i)
    {
        if (tirosInimigo[i])
        {
            tirosInimigo[i]->Move(dt);

            tirosInimigo[i]->setDebug(debugColisor);
            if (!tirosInimigo[i]->Valido())
            {
                delete tirosInimigo[i];
                tirosInimigo[i] = nullptr;
            }
            else if (tirosInimigo[i]->ColideCilindro3D(*tirosInimigo[i], personagem))
            {
                gameUI.Atingido();
                delete tirosInimigo[i];
                tirosInimigo[i] = nullptr;
            }
        }
        if (tiros[i])
        {
            tiros[i]->Move(dt);
            tiros[i]->setDebug(debugColisor);

            for (int b = qtBarril - 1; b >= 0; --b)
            {
                if (barris[b] && tiros[i]->ColideCilindro3D(*tiros[i], *barris[b]))
                {
                    barris[b]->Atingido();
                    gameUI.Ponto();
                    if (!barris[b]->Valido())
                        gameUI.BarrilDestruido();
                    delete tiros[i];
                    tiros[i] = nullptr;
                    break;
                }
            }
            if (tiros[i] && !tiros[i]->Valido())
            {
                delete tiros[i];
                tiros[i] = nullptr;
            }
        }
    }
}

static void desenhaTiros()
{
    for (int i = 0; i < qtTiro; ++i)
    {
        if (tiros[i])
            tiros[i]->Desenha();
        if (tirosInimigo[i])
            tirosInimigo[i]->Desenha();
    }
}

// ------------------------------------------------------------------
// Barris
// ------------------------------------------------------------------
static int barrilAlvoIdx()
{
    for (int i = 0; i < qtBarril; ++i)
        if (barris[i] != nullptr)
            return i;
    return -1;
}
static void controlaBarris(GLfloat dt)
{
    if (pause)
        return;
    GLfloat now = glutGet(GLUT_ELAPSED_TIME);
    GLfloat px, py;
    personagem.getPos(px, py);

    if (now > tempoCriarBarril)
    {
        tempoCriarBarril = now + FloatAleatorio(1000.0, 5000.0);
        GLfloat y = config.arena.altura / 2.0f;
        Barril *b = new Barril(ladoBarril, y, IntAleatorio(0, 1));
        b->Configurar(config);
        for (int i = 0; i < qtBarril; ++i)
        {
            if (barris[i] == nullptr)
            {
                ladoBarril *= -1;
                barris[i] = b;
                break;
            }
        }
    }
    for (int i = 0; i < qtBarril; ++i)
    {
        if (!barris[i])
            continue;

        barris[i]->setDebug(debugColisor);
        barris[i]->Mirar(px, py);
        // criaTiros(barris[i]->Atirar(now, VelocidadeTiro), tirosInimigo);
        barris[i]->Move(dt);

        if (barris[i]->ColideCilindro3D(*barris[i], personagem))
        {
            gameUI.Atingido();
            delete barris[i];
            barris[i] = nullptr;
            continue;
        }
        if (barris[i] && !barris[i]->Valido())
        {
            delete barris[i];
            barris[i] = nullptr;
        }
    }
}

static void desenhaBarris()
{
    for (int i = 0; i < qtBarril; ++i)
        if (barris[i])
            barris[i]->Desenha();
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

    camera_apply(); // 1) câmera
    capturaMatrizesCamera();
    lighting_apply(); // 2) luzes (na MODELVIEW da câmera)

    // 3) objetos 3D
    personagem.Desenha();
    arena.Desenha(); // <-- Adicione isso AQUI
    desenhaTiros();
    desenhaBarris();

    // debugTiro();
    // 4) HUD por cima (troca de projeção e volta)
    desenhaUI2D();

    glutSwapBuffers();
}

// ------------------------------------------------------------------
// Callbacks
// ------------------------------------------------------------------
void keyPress(unsigned char key, int, int)
{
    switch (key)
    {
    case 'r':
    case 'R':
        keyStatus['r'] = 1;
        break;
    case 'a':
    case 'A':
        keyStatus['a'] = 1;
        break;
    case 'd':
    case 'D':
        keyStatus['d'] = 1;
        break;
    case 'w':
    case 'W':
        keyStatus['w'] = 1;
        break;
    case 's':
    case 'S':
        keyStatus['s'] = 1;
        break;
    case 'q':
    case 'Q':
        keyStatus['q'] = 1;
        break;
    case 'e':
    case 'E':
        keyStatus['e'] = 1;
        break;
    case ' ':
        keyStatus[' '] = 1;
        break;

    case 'c':
    case 'C':
        camera_cycle_mode();
        break;
    case '+':
    case '=':
        camera_set_zoom(-50.0f);
        break;
    case '-':
    case '_':
        camera_set_zoom(50.0f);
        break;

    case '[':
        camera_set_base_height(camera_get_base_height() - 10.0f);
        break;
    case ']':

        camera_set_base_height(camera_get_base_height() + 10.0f);
        break;

    case '\t': // Tab (ASCII 9)
        camera_cycle_mode();
        camera_reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        break;

    // teclas 2/3/4 trocam preset de iluminação (útil para testar)
    case '2':
        lighting_preset_jogo();
        break;
    case '3':
        lighting_preset_estudio();
        break;
    case '4':
        lighting_preset_sol();
        break;
    case 'v':
    case 'V':
        debugColisor = !debugColisor;
        personagem.setDebug(debugColisor);
        break;
    case 'p':
    case 'P':
        pause = !pause;
        break;
    case 'f':
    case 'F':
        personagem.AlternarLanterna();
        break;
    case 27:
        exit(0);
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
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
            mousePressionado = true;
        if (state == GLUT_UP)
            mousePressionado = false;
    }
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            mouseBtnDown = 1;
            lastMouseX = x;
            lastMouseY = y;
        }
        if (state == GLUT_UP)
            mouseBtnDown = 0;
    }
}

void mouseAtivo(int x, int y)
{
    if (!mouseBtnDown)
        return;
    camera_orbit((float)(x - lastMouseX), (float)(y - lastMouseY));
    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

// ------------------------------------------------------------------
// Init
// ------------------------------------------------------------------
void init(void)
{
    for (int i = 0; i < 256; ++i)
        keyStatus[i] = 0;

    glClearColor(0.298f, 0.675f, 0.635f, 1.0f);
    // ---- Estado de textura ----
    glEnable(GL_TEXTURE_2D); // habilitar
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // corrige distorção em 3D
                                                       // ---- Estado de textura ----
    glDisable(GL_CULL_FACE);
    camera_init();
    lighting_init();
    lighting_preset_jogo();

    lastMouseX = Width / 2;
    lastMouseY = Height / 2;
    // Projeção inicial (reshape também fará isso no primeiro frame)
    camera_reshape(Width, Height);
}
void mousePassivo(int x, int y)
{
    // Delta em relação à última posição
    float dx = (float)(x - lastMouseX);
    float dy = (float)(y - lastMouseY);
    lastMouseX = x;
    lastMouseY = y;

    // Só gira a arma se houve movimento real
    if (dx != 0.0f || dy != 0.0f)
    {
        personagem.GirarArmaComMouse(dx, dy);
    }

    glutPostRedisplay();
}
// void mousePassivo(int x, int y)
// {
//     gMousePixelX = x;
//     gMousePixelY = y;
//     // A mira 3D real virá depois (gluUnProject); por ora só guarda o pixel.
//     glutPostRedisplay();
// }
void idle(void)
{
    GLfloat dt = deltaTime() / 1000.0f;
    int dx = 0, dy = 0;
    if (keyStatus['a'])
        personagem.Girar(dt);
    if (keyStatus['d'])
        personagem.Girar(-dt);
    if (keyStatus['w'])
    {
        personagem.Mover(dt);
        dy += 1;
    }
    if (keyStatus['s'])
    {
        personagem.Mover(-dt);
        dy -= 1;
    }
    if (keyStatus['q'])
    {
        personagem.MoverLado(-dt);
        dx += 1;
    }
    if (keyStatus['e'])
    {
        personagem.MoverLado(dt);
        dx -= 1;
    }

    if (keyStatus[' '] || mousePressionado)
    {
        GLdouble now = glutGet(GLUT_ELAPSED_TIME);
        if (now > tempoTiro + tiroPorSegundo)
        {
            tempoTiro = now;
            criaTiros(personagem.Atirar(VelocidadeTiro), tiros);
        }
    }
    if (keyStatus['r'])
        resetGame();
    controlaTiros(dt);
    controlaBarris(dt);
    personagem.SetDirecaoMovimento(dx, dy);
    personagem.AtualizaAnimacaoAnim(dt);
    InimigoCamera = barrilAlvoIdx();
    GLfloat px, py;
    switch (camera_get_mode())
    {
    case CAM_ENEMY_VIEW:
        if (InimigoCamera >= 0 && barris[InimigoCamera])
        {
            float bx, by, bz;
            barris[InimigoCamera]->GetPosition(bx, by, bz);
            camera_instance().setPosition(bx, by, bz + 100);
            float pxp, pyp;
            personagem.getPos(pxp, pyp);
            camera_instance().setTarget(pxp, pyp, 100);
        }
        else
        {

            // fallback: olha pro personagem com a câmera de cima
            personagem.getPos(px, py);
            camera_instance().setPosition(px, py - 300.0f, 250.0f);
            camera_instance().setTarget(px, py, 0.0f);
        }
        break;

    case CAM_FIRST_PERSON:
    case CAM_THERD_PERSON:
        personagem.getPos(px, py);
        camera_set_target(px, py, 5.0f);
        camera_set_direcao(personagem.getAngulo());
        break;

    default:
        personagem.getPos(px, py);
        camera_set_target(px, py, 5.0f);
        // camera_set_direcao(personagem.getAngulo());
        break;
    }

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

    if (config.tiro.tiroPorSegundo)
        tiroPorSegundo = 1000.0 / config.tiro.tiroPorSegundo;
    if (config.tiro.velocidade)
        VelocidadeTiro = config.tiro.velocidade;
    if (config.barril.velocidade)
        velocidadeInimigo = config.barril.velocidade;

    if (config.arena.largura)
        ladoBarril = config.arena.largura / 4;

    arena.Configurar(config);
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
    arena.CarregarTexturas();

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