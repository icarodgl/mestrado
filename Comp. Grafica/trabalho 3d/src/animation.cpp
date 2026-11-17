
#include "animation.h"
#include <stdio.h>
#include <math.h>

bool Animation::load(const char* pathPattern, int firstIdx, int lastIdx)
{
    m_frames.clear();
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
        m_frames.push_back(std::move(m));
    }

    if (m_frames.empty()) {
        fprintf(stderr, "Animation: nenhum frame carregado de %s\n", pathPattern);
        return false;
    }
    return true;
}

void Animation::rebuildCurrent()
{
    if (m_frames.empty()) { m_current = 0; return; }

    float dur = duration();
    if (dur <= 0.0f) { m_current = 0; return; }

    if (m_loop) {
        // wrap em [0, dur)
        float t = fmodf(m_time, dur);
        m_current = (int)(t * m_fps);
        if (m_current >= (int)m_frames.size()) m_current = 0;
    } else {
        if (m_time >= dur) {
            m_current = (int)m_frames.size() - 1;   // congela no ultimo
        } else {
            m_current = (int)(m_time * m_fps);
        }
    }
}

void Animation::update(float dt)
{
    if (m_frames.empty()) return;
    m_time += dt;
    rebuildCurrent();
}

void Animation::draw() const
{
    if (m_frames.empty()) return;
    m_frames[m_current].draw();
}