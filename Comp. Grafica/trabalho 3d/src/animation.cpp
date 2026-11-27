#include "animation.h"
#include <stdio.h>
#include <math.h>

bool Animation::load(const char* pathPattern, int firstIdx, int lastIdx) {
    m_ownedFrames.clear();
    m_time = 0.0f;
    m_current = 0;

    for (int i = firstIdx; i <= lastIdx; ++i) {
        char path[512];
        snprintf(path, sizeof(path), pathPattern, i);
        mesh m;
        if (!m.loadMesh(path)) {
            fprintf(stderr, "Animation: falha ao carregar %s\n", path);
            continue;
        }
        m_ownedFrames.push_back(std::move(m));
    }
    m_frames = &m_ownedFrames;
    return !m_ownedFrames.empty();
}

void Animation::rebuildCurrent() {
    if (empty()) { m_current = 0; return; }

    float dur = duration();
    if (dur <= 0.0f) { m_current = 0; return; }

    if (m_loop) {
        float t = fmodf(m_time, dur);
        m_current = (int)(t * m_fps);
        if (m_current >= (int)m_frames->size()) m_current = 0;
    } else {
        if (m_time >= dur) m_current = (int)m_frames->size() - 1;
        else               m_current = (int)(m_time * m_fps);
    }
}

void Animation::update(float dt) {
    if (empty()) return;
    if (dt <= 0.0f) return;
    m_time += dt;
    rebuildCurrent();
}

void Animation::draw(GLfloat texture) const {
    if (empty()) return;
    (*m_frames)[m_current].draw(texture);
}