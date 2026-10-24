#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <GL/freeglut_std.h>

class MyGameUI
{
    int vida;
    GLfloat vidaPosX = 180.0;
    GLfloat vidaPosY = 200.0;

private:
    void DesenhaVida() const
    {
        std::string txt = std::to_string(vida);
    }
    void DesenhaTexto(float x, float y, const std::string &texto, void *fonte) const
    {
        glRasterPos2f(x, y);
        for (char c : texto)
        {
            glutBitmapCharacter(fonte, c);
        }
    };
    void DesenhaGameOver()
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        std::string txt = "Game Over!";
        glPushMatrix();
        glTranslatef(0, 0, 0);
        glRecti(-250, -250, 250, 250);
        glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        DesenhaTexto(-20, 0, txt, GLUT_BITMAP_HELVETICA_18);
    }
    void DesenhaVida()
    {
        std::string txt = "Vidas: " + std::to_string(vida);
        glColor3f(1.0f, 1.0f, 1.0f);
        DesenhaTexto(vidaPosX, vidaPosX, txt, GLUT_BITMAP_HELVETICA_18);
    }

public:
    MyGameUI()
    {
        vida = 5;
    }
    void Atingido()
    {
        if (vida > 0)
            vida -= 1;
    };
    void Desenha()
    {
        if (vida <= 0)
        {
            DesenhaGameOver();
        }
        DesenhaVida();
    }
};