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

#define INC_KEY 1
#define INC_KEYIDLE 5.0

Configuracao cfg;
// Key status
int keyStatus[256];

// Window dimensions
const GLint Width = 700;
const GLint Height = 700;

// Viewing dimensions
const GLint ViewingWidth = 500;
const GLint ViewingHeight = 500;

static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
// Controla a animacao do robo
int animate = 0;

// Componentes do mundo virtual sendo modelado
Personagem personagem;
MyGameUI gameUI; 

static const int qtTiro = 20;
static const int qtBarril = 6;
GLfloat tempoCriarBarril = 0.0;
GLfloat tempoInimigoAtirar = 0.0;
int ladoBarril = 100;
GLfloat tempoTiro = 0.0;
Tiro *tiros[qtTiro] = {};
Tiro *tirosInimigo[qtTiro] = {};
Barril *barris[qtBarril] = {};

GLdouble deltaTime()
{
    GLdouble currentTime, timeDiference;
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    timeDiference = currentTime - previousTime;
    previousTime = currentTime;

    return timeDiference;
}

void criaTiros(Tiro *t, Tiro* arrTiro[])
{
    if (t == nullptr)
        return;

    for (int i = 0; i < qtTiro; ++i)
        if (arrTiro[i] == nullptr)
        {
            arrTiro[i] = t;
            return;
        }
    delete t;
}
void controlaTiros()
{
    GLfloat deltaT = deltaTime();

    for (int i = 0; i < qtTiro; ++i)
    {
        // ==========================================
        // Tiro inimigo
        // ==========================================

        if (tirosInimigo[i] != nullptr){

            tirosInimigo[i]->Move(deltaT);

            if (!tirosInimigo[i]->Valido())
                {
                    delete tirosInimigo[i];
                    tirosInimigo[i] = nullptr;
                }

            if(tirosInimigo[i] != nullptr and tirosInimigo[i]->ColideCom(personagem)){
                gameUI.Atingido();
                delete tirosInimigo[i];
                tirosInimigo[i] = nullptr;
            }

        }
        // ==========================================
        // Tiro Aliado
        // ==========================================
        if (tiros[i] != nullptr)
        {
            tiros[i]->Move(deltaT);
            
            for (int barril = qtBarril- 1; barril >= 0; barril--)
            {
                if(barris[barril] != nullptr && tiros[i]->ColideCom(*barris[barril])){
                    barris[barril]->Atingido();
                    gameUI.Ponto();
                    delete tiros[i];
                    tiros[i] = nullptr;
                    break;
                }
            }
        
            if (tiros[i] != nullptr and !tiros[i]->Valido())
            {
                delete tiros[i];
                tiros[i] = nullptr;
            }
        }
    }
}
void desenhaTiros()
{
    for (int i = 0; i < qtTiro; ++i)
    {
        if (tiros[i] != nullptr)
        {
            tiros[i]->Desenha();
        }
        if (tirosInimigo[i] != nullptr)
        {
            tirosInimigo[i]->Desenha();
        }
    }
}
void controlaBarris()
{
    GLfloat currentTime = glutGet(GLUT_ELAPSED_TIME);
    GLfloat deltaT = deltaTime();
    
    if (currentTime > tempoCriarBarril + FloatAleatorio(1000.0, 5000.0))
    {
        tempoCriarBarril = currentTime;
        GLfloat y = 220.0;
        Barril *b = new Barril(ladoBarril, y,IntAleatorio(2,5), IntAleatorio(0,1));
        
        for (int i = 0; i < qtBarril; ++i)
        {
            if (barris[i] == nullptr)
            {
                ladoBarril = ladoBarril * -1;
                barris[i] = b;
                break;
            }
        }
    }

    for (int i = 0; i < qtBarril; ++i)
    {
        if (barris[i] != nullptr)
        {
        // ==========================================
        // Cria Tiro inimigo
        // ==========================================
            criaTiros(barris[i]->Atirar(currentTime),tirosInimigo);
        // ==========================================
        // Move inimigo
        // ==========================================
            barris[i]->Move(deltaT);

        // ==========================================
        // Testa colizao
        // ==========================================
            if (barris[i]->ColideCom(personagem)){
                gameUI.Atingido();
                delete barris[i];
                barris[i] = nullptr;
                break;
            }

            if (barris[i] != nullptr && !barris[i]->Valido())
            {
                delete barris[i];
                barris[i] = nullptr;
            }
        }
    }
}

void desenhaBarris()
{
    for (int i = 0; i < qtBarril; ++i)
    {
        if (barris[i] != nullptr)
        {
            barris[i]->Desenha();
        }
    }
}
void renderScene(void)
{
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT);

    personagem.Desenha();

    desenhaTiros();
    desenhaBarris();
    gameUI.Desenha();

    glutSwapBuffers(); // Desenha the new frame of the game.
}

void keyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        animate = !animate;
        break;
    case 'a':
    case 'A':
        keyStatus[(int)('a')] = 1; // Using keyStatus trick
        break;
    case 'd':
    case 'D':
        keyStatus[(int)('d')] = 1; // Using keyStatus trick
        break;
    case 'w':
    case 'W':
        keyStatus[(int)('w')] = 1; // Using keyStatus trick
        break;
    case 's':
    case 'S':
        keyStatus[(int)('s')] = 1; // Using keyStatus trick
        break;
    case 'q':
    case 'Q':
        keyStatus[(int)('q')] = 1; // Using keyStatus trick
        break;
    case 'E':
    case 'e':
        keyStatus[(int)('e')] = 1; // Using keyStatus trick
        break;
    case ' ':
        keyStatus[(int)(' ')] = 1;
     
        break;
    case 27:
        exit(0);
    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void ResetKeyStatus()
{
    int i;
    // Initialize keyStatus
    for (i = 0; i < 256; i++)
        keyStatus[i] = 0;
}

void init(void)
{
    ResetKeyStatus();
    // The color the windows will redraw. Its done to erase the previous frame.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black, no opacity(alpha).

    glMatrixMode(GL_PROJECTION);  // Select the projection matrix
    glOrtho(-(ViewingWidth / 2),  // X coordinate of left edge
            (ViewingWidth / 2),   // X coordinate of right edge
            -(ViewingHeight / 2), // Y coordinate of bottom edge
            (ViewingHeight / 2),  // Y coordinate of top edge
            -100,                 // Z coordinate of the “near” plane
            100);                 // Z coordinate of the “far” plane
    glMatrixMode(GL_MODELVIEW);   // Select the projection matrix
    glLoadIdentity();
}

void idle(void)
{
    double inc = INC_KEYIDLE;

    if (keyStatus[(int)('a')])
    {
        personagem.Girar(inc);
    }
    if (keyStatus[(int)('d')])
    {
        personagem.Girar(-inc);
    }
    if (keyStatus[(int)('w')])
    {
        personagem.Mover(inc);
    }
    if (keyStatus[(int)('s')])
    {
        personagem.Mover(-inc);
    }
    if (keyStatus[(int)('q')])
    {
        personagem.MoverLado(-inc);
    }
    if (keyStatus[(int)('e')])
    {
        personagem.MoverLado(inc);
    }
    if (keyStatus[(int)(' ')]){
        GLdouble currentTime = glutGet(GLUT_ELAPSED_TIME);
        if (currentTime > tempoTiro + 200.0)
        {
            tempoTiro = currentTime;
            criaTiros(personagem.Atirar(), tiros);
        }
    }
    ResetKeyStatus();

    controlaTiros();
    controlaBarris();

    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.

    if (!CarregaConfiguracao("configuracoes.xml", cfg))
        fprintf(stderr, "Usando valores padrão.\n");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Create the window.
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150, 50);
    glutCreateWindow("Tranformations 2D");

    // Define callbacks.
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyPress);
    glutIdleFunc(idle);
    glutKeyboardUpFunc(keyup);

    init();

    glutMainLoop();

    return 0;
}
