#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include <string>
#include "objloader.h"

class Animation {
public:
    void setFrames(const std::vector<mesh>* frames) { m_frames = frames; }

    bool load(const char* pathPattern, int firstIdx, int lastIdx);

    void update(float dt);
    void draw(GLfloat texture) const;

    void reset() { m_time = 0.0f; }

    void  setFPS(float fps)  { m_fps = (fps > 0.0f) ? fps : 1.0f; }
    float fps() const        { return m_fps; }
    void  setLoop(bool loop) { m_loop = loop; }
    bool  finished() const   { return !m_loop && m_time >= duration(); }

    int  frameCount() const   { return m_frames ? (int)m_frames->size() : 0; }
    int  currentFrame() const { return m_current; }
    bool empty() const        { return !m_frames || m_frames->empty(); }

    const mesh* getCurrentMesh() const {
        if (empty()) return nullptr;
        return &(*m_frames)[m_current];
    }

private:
    const std::vector<mesh>* m_frames = nullptr;  
    std::vector<mesh>        m_ownedFrames;     

    float m_time    = 0.0f;
    float m_fps     = 12.0f;
    bool  m_loop    = true;
    int   m_current = 0;

    float duration() const {
        return empty() ? 0.0f : (float)m_frames->size() / m_fps;
    }
    void rebuildCurrent();
};

#endif