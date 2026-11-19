
#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>
#include "objloader.h"

class Animation {
public:
    // Carrega frames com path tipo "assets/andando/passo%d.obj"
    // de firstIdx ate lastIdx (inclusive). Ex: load(".../passo%d.obj", 1, 10)
    bool load(const char* pathPattern, int firstIdx, int lastIdx);

    // Avanca o tempo em dt segundos. Se loop=true, cicla; senao, para no ultimo.
    void update(float dt);

    // Desenha o frame atual com as transformacoes JA APLICADAS pelo chamador
    // (a Animation nao mexe na matrix; soh escolhe o mesh e chama draw()).
    void draw() const;

    // Reset para o frame 0
    void reset() { m_time = 0.0f; }

    // Controle
    void setFPS(float fps)         { m_fps = (fps > 0.0f) ? fps : 1.0f; }
    float fps() const              { return m_fps; }
    void setLoop(bool loop)        { m_loop = loop; }
    bool finished() const          { return !m_loop && m_time >= duration(); }

    // Info
    int  frameCount() const        { return (int)m_frames.size(); }
    int  currentFrame() const      { return m_current; }
    bool empty() const             { return m_frames.empty(); }
    const mesh* getCurrentMesh() const {
        if (m_frames.empty()) return nullptr;
        return &m_frames[m_current];
    }
private:
    std::vector<mesh> m_frames;
    float m_time    = 0.0f;
    float m_fps     = 12.0f;
    bool  m_loop    = true;
    int   m_current = 0;

    float duration() const {
        return m_frames.empty() ? 0.0f : (float)m_frames.size() / m_fps;
    }
    void  rebuildCurrent();
};

#endif