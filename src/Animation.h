/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef ANIMATION_H
#define ANIMATION_H

#include "Image.h"
#include <vector>

class Animation
{
 public:
    Animation()
        : m_currentTime(0.0f)
        , m_fullTime(0.0f)
        , m_reverse(false)
        , m_playingBack(false)
        , m_currentFrameIndex(0)
    {}

    void update(float delta)
    {
        m_currentTime += delta;
        if (m_currentTime > m_fullTime) {
            m_currentTime -= m_fullTime;
            m_playingBack = !m_playingBack;
        }

        if (m_reverse && m_playingBack) {
            m_currentFrameIndex = m_frames.size() - 1;
            float t = 0.0f;
            for (int i = m_frameDurations.size() - 1;  i >= 0; --i) {
                t += m_frameDurations[i];
                if (t < m_currentTime)
                    --m_currentFrameIndex;
            }

        } else {
            m_currentFrameIndex = 0;
            float t = 0.0f;
            for (float dur : m_frameDurations) {
                t += dur;
                if (t < m_currentTime)
                    ++m_currentFrameIndex;
            }
        }
    }

    const Image& currentFrame()
    {
        return m_frames[m_currentFrameIndex];
    }

    void addFrame(const Image& image, float duration = 1.0f)
    {
        m_frames.push_back(image);
        m_frameDurations.push_back(duration);

        // Recalculate full time
        m_fullTime = 0.0f;
        for (float dur : m_frameDurations)
            m_fullTime += dur;
    }

    void setReversable(bool reversable) { m_reverse = reversable; }

 private:
    std::vector<Image> m_frames;
    std::vector<float> m_frameDurations;
    float m_currentTime;
    float m_fullTime;
    bool m_reverse;
    bool m_playingBack;
    int m_currentFrameIndex;
};

#endif // ANIMATION_H
