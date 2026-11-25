#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>
#include "objloader.h"

class Animation {
public:

    bool load(const char* pathPattern, int firstIdx, int lastIdx);

    void update(float dt);

    void draw(GLfloat texture) const;

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