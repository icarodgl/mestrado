#include "arena.h"
#include "texture.h"
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>


Arena::Arena() {
    largura = 500.0f;
    profundidade = 150.0f;
    alturaParede = 30.0f;
}

Arena::~Arena() {
    if (texturaParede) glDeleteTextures(1, &texturaParede);
    if (texturaChao)   glDeleteTextures(1, &texturaChao);
}

void Arena::Configurar(const Configuracao& conf) {
    if (conf.arena.largura > 0) largura = (float)conf.arena.largura;
    if (conf.arena.altura > 0)  profundidade = (float)conf.arena.altura;
    if (conf.jogador.raioCabeca > 0)
        alturaParede = (float)conf.jogador.raioCabeca;
    else
        alturaParede = 30.0f;
    raioAreaCentral = largura * 0.20f; 
}

void Arena::CarregarTexturas() {
    if (!texturaParede) {
        texturaParede = loadBMP("src/assets/parede1.bmp");
        if (texturaParede == 0)
            fprintf(stderr, "Arena: FALHA ao carregar textura da parede\n");
        else
            printf("Arena: parede OK texId=%u\n", texturaParede);
    }
    if (!texturaChao) {
        texturaChao = loadBMP("src/assets/piso2.bmp");
        if (texturaChao == 0)
            fprintf(stderr, "Arena: FALHA ao carregar textura do chao\n");
        else
            printf("Arena: chao OK texId=%u\n", texturaChao);
    }
    GaranteMalhaCone();
}

void Arena::GaranteMalhaCone() {
    if (coneCarregado) return;

    // Ajuste o caminho para o local real do seu cone.obj
    if (!malhaCone.loadMesh("src/assets/cone.obj")) {
        fprintf(stderr, "Arena: falha ao carregar cone.obj\n");
    }

    if (!malhaCone.vertsPos.empty()) {
        float mn[3] = { 1e30f, 1e30f, 1e30f};
        float mx[3] = {-1e30f,-1e30f,-1e30f};
        for (auto& v : malhaCone.vertsPos) {
            mn[0]=fminf(mn[0],v.x); mn[1]=fminf(mn[1],v.y); mn[2]=fminf(mn[2],v.z);
            mx[0]=fmaxf(mx[0],v.x); mx[1]=fmaxf(mx[1],v.y); mx[2]=fmaxf(mx[2],v.z);
        }
        minYMalhaCone = mn[1];

        // Define a escala para o cone ter uns 25 unidades de altura
        float altura = mx[1] - mn[1];
        float maior = fmaxf(altura, fmaxf(mx[0]-mn[0], mx[2]-mn[2]));
        escalaCone = (maior > 1e-4f) ? (25.0f / maior) : 1.0f;
    }
    coneCarregado = true;
}
void Arena::DesenhaCone(float x, float y) {
    if (malhaCone.vertsPos.empty()) return;

    glPushMatrix();
    
    // 1) Posiciona no plano XY
    glTranslatef(x, y, 0.0f);

    // 2) Deita o modelo do Blender (Y-up) para o plano XY do jogo
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    // 3) Escala e apoio no chão
    glScalef(escalaCone, escalaCone, escalaCone);
    glTranslatef(0.0f, -minYMalhaCone, 0.0f);

    // 4) Cor (caso não tenha textura)
    GLfloat matCone[] = { 1.0f, 0.5f, 0.0f, 1.0f }; // Laranja
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matCone);

    // 5) Desenha
    malhaCone.draw();

    glPopMatrix();
}

void Arena::DesenhaLinhaDeConesArena()const{
if (!malhaCone.vertsPos.empty()) {
            float cx = -largura/2;
            float cy = 0;
        for (int i = 0; i < numCones; ++i) {
            const_cast<Arena*>(this)->DesenhaCone(cx, cy);
            cx += largura/numCones;
        }
    }
}
void Arena::Desenha() const {
    float w = largura / 2.0f;
    float d = profundidade / 2.0f;
    float h = 256.0;

    DesenhaLinhaDeConesArena();
    // ---- Chão ----
    GLfloat matChao[] = { 0.2f, 0.6f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matChao);

    if (texturaChao) {
        texture_enable();
        glBindTexture(GL_TEXTURE_2D, texturaChao);
        // repete a textura proporcionalmente ao tamanho do chão
        float repX = 5.0;//largura  / 100.0f; // 100 = tamanho de 1 tile
        float repY = 5.0;//profundidade / 100.0f;
        glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);
            glTexCoord2f(0.0f,   0.0f);   glVertex3f(-w, -d, 0);
            glTexCoord2f(repX,   0.0f);   glVertex3f( w, -d, 0);
            glTexCoord2f(repX,   repY);   glVertex3f( w,  d, 0);
            glTexCoord2f(0.0f,   repY);   glVertex3f(-w,  d, 0);
        glEnd();
        texture_disable();
    } else {
        glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);
            glVertex3f(-w, -d, 0);
            glVertex3f( w, -d, 0);
            glVertex3f( w,  d, 0);
            glVertex3f(-w,  d, 0);
        glEnd();
    }

    // ---- Paredes ----
    GLfloat matParede[] = { 1.0f, 1.0f, 1.0f, 1.1f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matParede);

   


    bool usaTextura = (texturaParede != 0);
    if (usaTextura) {
        texture_enable();
        glBindTexture(GL_TEXTURE_2D, texturaParede);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    // Repetições horizontais/verticais por parede
    // (dimensão real / tamanho do tile em unidades)
    float tileSize = 256.0f;
    float repH = h       / tileSize;   // repetição vertical
    float repS = largura / tileSize;   // parede Sul/Norte (largura)
    float repE = profundidade / tileSize; // parede Leste/Oeste (profundidade)
    float ZERO = 0.0;

    glBegin(GL_QUADS);

    // ---- Parede Sul (Y = -d) ----
    glNormal3f(0, 1, 0);
    glTexCoord2f(ZERO,    repH);   glVertex3f(-w, -d, 0);
    glTexCoord2f(repS,    repH);   glVertex3f( w, -d, 0);
    glTexCoord2f(repS,    ZERO);   glVertex3f( w, -d, h);
    glTexCoord2f(ZERO,    ZERO);   glVertex3f(-w, -d, h);

    // =================================================================
    // PAREDE NORTE (Fundo da arena, Y = d)
    // =================================================================
    glNormal3f(0, -1, 0);
    // Base esquerda (U=0, V=0) -> Subindo para o topo esquerdo (U=0, V=repH)
    // Topo direito (U=repS, V=repH) -> Descendo para a base direita (U=repS, V=0)
    glTexCoord2f(repS,    repH);   glVertex3f(-w, d, 0); // Base Esquerda
    glTexCoord2f(repS,    ZERO);   glVertex3f(-w, d, h); // Topo Esquerdo
    glTexCoord2f(ZERO,    ZERO);   glVertex3f( w, d, h); // Topo Direito
    glTexCoord2f(ZERO,    repH);   glVertex3f( w, d, 0); // Base Direita

    // =================================================================
    // PAREDE OESTE (Lado esquerdo, X = -w)
    // =================================================================
    glNormal3f(1, 0, 0);
    // Base frente (U=0, V=0) -> Sobe -> Desce pela base de trás (U=repE, V=0)
    // Nota: O eixo U aqui corre ao longo do eixo Y do mundo (profundidade)
    glTexCoord2f(repE,    repH);   glVertex3f(-w, -d, 0); // Base Frente
    glTexCoord2f(repE,    ZERO);   glVertex3f(-w, -d, h); // Topo Frente
    glTexCoord2f(ZERO,    ZERO);   glVertex3f(-w,  d, h); // Topo Fundo
    glTexCoord2f(ZERO,    repH);   glVertex3f(-w,  d, 0); // Base Fundo

    // =================================================================
    // PAREDE LESTE (Lado direito, X = w)
    // =================================================================
    glNormal3f(-1, 0, 0);
    // Base de trás (U=0, V=0) -> Sobe -> Desce pela base da frente (U=repE, V=0)
    glTexCoord2f(repE,    repH);   glVertex3f(w, d, 0);   // Base Fundo
    glTexCoord2f(repE,    ZERO);   glVertex3f(w, d, h);   // Topo Fundo
    glTexCoord2f(ZERO,    ZERO);   glVertex3f(w, -d, h);  // Topo Frente
    glTexCoord2f(ZERO,    repH);   glVertex3f(w, -d, 0);  // Base Frente

    glEnd();

    if (usaTextura) texture_disable();
}