#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "barril.h"
#include "tiro.h"
#include "personagem.h"
#define INC_KEY 1
#define INC_KEYIDLE 0.01

//Key status
int keyStatus[256];

// Window dimensions
const GLint Width = 700;
const GLint Height = 700;

// Viewing dimensions
const GLint ViewingWidth = 500;
const GLint ViewingHeight = 500;

static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
//Controla a animacao do robo
int animate = 0;

//Componentes do mundo virtual sendo modelado
Personagem personagem;

const int qtTiro = 20;
const int qtBarril = 20;
Tiro* tiros[qtTiro] = {nullptr};
Barril* barris[qtBarril] = {nullptr};


GLdouble deltaTime(){
    GLdouble currentTime, timeDiference;
    //Pega o tempo que passou do inicio da aplicacao
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    // Calcula o tempo decorrido desde de a ultima frame.
    timeDiference = currentTime - previousTime;
    //Atualiza o tempo do ultimo frame ocorrido
    previousTime = currentTime;

    return timeDiference;
}


void criaTiros(Tiro* t){

}
void controlaTiros(){
   for (int i = 0; i < qtTiro; ++i)
    {
        if (tiros[i] != nullptr)
        {
            tiros[i]->Move(deltaTime());
            if(!tiros[i]->Valido()){
                delete tiros[i];
                tiros[i] = nullptr;
            }
        }
    }
}
void desenhaTiros(){
    for (int i = 0; i < qtTiro; ++i)
    {
        if (tiros[i] != nullptr)
        {
            tiros[i]->Desenha();
        }
    }
}
void controlaBarris(){
       for (int i = 0; i <  qtBarril; ++i)
    {
        if (barris[i] != nullptr)
        {
            barris[i]->Move(deltaTime());
            if(!barris[i]->Valido()){
                delete barris[i];
                barris[i] = nullptr;
            }
        }
    }
}

void desenhaBarris(){
    for (int i = 0; i <  qtBarril; ++i)
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
             keyStatus[(int)('a')] = 1; //Using keyStatus trick
             personagem.Girar(1);
             break;
        case 'd':
        case 'D':
             keyStatus[(int)('d')] = 1; //Using keyStatus trick
             personagem.Girar(-1);
             break;
        case ' ':
            criaTiros(personagem.Atirar());
            break;
        case 27 :
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
    //Initialize keyStatus
    for(i = 0; i < 256; i++)
       keyStatus[i] = 0; 
}

void init(void)
{
    ResetKeyStatus();
    // The color the windows will redraw. Its done to erase the previous frame.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black, no opacity(alpha).
 
    glMatrixMode(GL_PROJECTION); // Select the projection matrix    
    glOrtho(-(ViewingWidth/2),     // X coordinate of left edge             
            (ViewingWidth/2),     // X coordinate of right edge            
            -(ViewingHeight/2),     // Y coordinate of bottom edge             
            (ViewingHeight/2),     // Y coordinate of top edge             
            -100,     // Z coordinate of the “near” plane            
            100);    // Z coordinate of the “far” plane
    glMatrixMode(GL_MODELVIEW); // Select the projection matrix    
    glLoadIdentity();
      
}

void idle(void)
{



    // double inc = INC_KEYIDLE;
    // //Treat keyPress
    // if(keyStatus[(int)('a')])
    // {
    //     // robo.MoveEmX(-inc);
    // }
    // if(keyStatus[(int)('d')])
    // {
    //     // robo.MoveEmX(inc);
    // }
    
    controlaTiros();
    controlaBarris();
    
    glutPostRedisplay();
}
 
int main(int argc, char *argv[])
{
    // Initialize openGL with Double buffer and RGB color without transparency.
    // Its interesting to try GLUT_SINGLE instead of GLUT_DOUBLE.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
 
    // Create the window.
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(150,50);
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
