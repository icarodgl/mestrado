#ifndef PERSONAGEM_H
#define PERSONAGEM_H
#include <GL/gl.h>
#include <GL/glu.h>
#include "tiro.h"
#include "colidivel.h"
#include "configuracao.h"
#include "animation.h"
#include "colidivel_cilindro.h"

class Personagem : public ColidivelCilindro
{
    enum DirecaoMovimento
    {
        DIR_NENHUMA,
        DIR_FRENTE,
        DIR_TRAS,
        DIR_ESQ,
        DIR_DIR
    };
    int vida;
    GLfloat gX = 0;
    GLfloat gY = -180;
    GLfloat ultimoX = 0.0f;
    GLfloat ultimoY = 0.0f;
    GLfloat fasePasso = 0.0f;
    GLfloat theta = 0;
    GLfloat pernas = 40;
    GLfloat velocidade = 5.0;
    GLfloat raioCabeca = 30.0;
    // arena
    int larguraArena = 200;
    int alturaArena = 150;
    int CorrecaoArenaY = 200;
    // 3d
    Animation animAndandoFrente;
    Animation animAndandoTras;
    Animation animAndandoEsq;
    Animation animAndandoDir;
    Animation animParado;

    // sensibilidade da mira
    GLfloat sensibilidadeMouseYaw = 0.15f; // graus por pixel
    GLfloat sensibilidadeMousePitch = 0.10f;
    // limites do pitch
    GLfloat thetaArma = 0;
    GLfloat thetaArmaPitch = 0.0f; // pitch da arma (graus, -90..+90)
    GLfloat pitchMin = -45.0f;
    GLfloat pitchMax = 60.0f;
    static const int PONTO_MAO = 1234;
    static const int PONTO_APONTA = 5678;

    DirecaoMovimento direcaoAtual = DIR_NENHUMA;
    bool animCarregada = false;
    bool emMovimento = false;
    float tempoSemMover = 0.0f;

    // debug
    float m_debugTiroSX = 0, m_debugTiroSY = 0, m_debugTiroSZ = 0;
    float m_debugTiroDX = 0, m_debugTiroDY = 0, m_debugTiroDZ = 0;
    float m_debugTiroPX = 0, m_debugTiroPY = 0, m_debugTiroPZ = 0;
    bool  m_debugTiroValido = false;

private:
    void desenhaDebugTiro();
    void DesenhaPersonagem(GLfloat x, GLfloat y);
    GLfloat CalculaBalancoPasso(GLfloat amplitude) const;
    void AtualizaAnimacao(GLfloat dt);
    void DesenhaArma();
    static void normalize(float a[3])
    {
        float n = sqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
        if (n < 1e-8f)
            return;
        a[0] /= n;
        a[1] /= n;
        a[2] /= n;
    }
    static void cross3(const float a[3], const float b[3], float out[3])
    {
        out[0] = a[1] * b[2] - a[2] * b[1];
        out[1] = a[2] * b[0] - a[0] * b[2];
        out[2] = a[0] * b[1] - a[1] * b[0];
    }
    static void ChangeCoordSys(
        GLdouble ax, GLdouble ay, GLdouble az,
        GLdouble bx, GLdouble by, GLdouble bz,
        GLdouble upx, GLdouble upy, GLdouble upz);

public:
    Personagem() : ColidivelCilindro(0, -180, 0, 10, 80) {};
    void GaranteAnimacao();                   // carrega lazily (primeira vez)
    void AtualizaAnimacaoAnim(float dt);      // avança e troca estado
    void SetDirecaoMovimento(int dx, int dy); // dx/dy em {-1, 0, 1}
    void Configurar(Configuracao conf)
    {
        raioCabeca = conf.jogador.raioCabeca;
        pernas = raioCabeca * 1.4;
        velocidade = conf.jogador.velocidade;
        vida = conf.jogador.vida;
        alturaArena = conf.arena.altura;
        larguraArena = conf.arena.largura;
        GaranteAnimacao();
    }

    void Desenha();
    bool Atingido(Colidivel *obj);
    void GirarArma(int x, int y);
    void Mover(GLfloat dir);
    void MoverLado(GLfloat dir);
    void Girar(GLfloat dir)
    {
        theta += dir * velocidade;
        SetRotateColisorZ(theta);
    }
    void getPos(GLfloat &x, GLfloat &y)
    {
        x = gX;
        y = gY;
    }
    GLfloat getAngulo() const { return theta; }
    void getDirecao(GLfloat &dx, GLfloat &dy) const
    {
        GLfloat rad = theta * M_PI / 180.0f;
        dx = -sinf(rad);
        dy = cosf(rad);
    }
    Tiro *Atirar(GLfloat velocidade);
    virtual void GetColider(GLfloat &x, GLfloat &y, GLfloat &w, GLfloat &h) const override
    {
        x = gX - raioCabeca;
        y = gY - raioCabeca;
        w = gX + raioCabeca;
        h = gY + raioCabeca;
    };
    void GirarArmaPitch(GLfloat dir)
    {
        thetaArmaPitch += dir;
        if (thetaArmaPitch > 60.0f)
            thetaArmaPitch = 60.0f;
        if (thetaArmaPitch < -45.0f)
            thetaArmaPitch = -45.0f;
    }
    // Define a mira 3D absoluta (usado em 1ª pessoa com unproject)
    void SetMira3D(float yawGraus, float pitchGraus)
    {
        theta = yawGraus;
        thetaArmaPitch = pitchGraus;
        SetRotateColisorZ(theta);
    }

    // Mira 2D: só yaw, zera pitch
    void SetMira3D(float alvoX, float alvoY, float /*ignorar*/)
    {
        float dx = alvoX - gX;
        float dy = alvoY - gY;
        float yaw = atan2f(dy, dx) * 180.0f / M_PI; // yaw "matemático" (0 => +X)
        // converte para o sistema do personagem (0 => +Y, frente)
        yaw -= 90.0f;
        while (yaw > 180.0f)
            yaw -= 360.0f;
        while (yaw < -180.0f)
            yaw += 360.0f;

        theta = yaw;
        thetaArmaPitch = 0.0f;
        SetRotateColisorZ(theta);
    }

    float getPitchArma() const { return thetaArmaPitch; }
    float getYawArma() const { return thetaArma; }
    void ResetarMiraArma()
    {
        thetaArma = 0.0f;
        thetaArmaPitch = 0.0f;
    }
    void GirarArmaComMouse(float dxPixels, float dyPixels)
    {
        thetaArma += dxPixels * sensibilidadeMouseYaw;
        thetaArmaPitch -= dyPixels * sensibilidadeMousePitch;

        if (thetaArmaPitch > pitchMax)
            thetaArmaPitch = pitchMax;
        if (thetaArmaPitch < pitchMin)
            thetaArmaPitch = pitchMin;

        while (thetaArma > 180.0f)
            thetaArma -= 360.0f;
        while (thetaArma < -180.0f)
            thetaArma += 360.0f;
    }
    bool debugTiroValido(){return m_debugTiroValido;}
    void getdebugTiro(float &sx, float &sy, float &sz,
                   float &dx, float &dy, float &dz,
                   float &px, float &py, float &pz) const {
        sx = m_debugTiroSX; sy = m_debugTiroSY; sz = m_debugTiroSZ;
        dx = m_debugTiroDX; dy = m_debugTiroDY; dz = m_debugTiroDZ;
        px = m_debugTiroPX; py = m_debugTiroPY; pz = m_debugTiroPZ;
    }
};
#endif
