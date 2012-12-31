#include "FpsCounter.h"
#include <iostream>

FpsCounter::FpsCounter(float printInterval)
    : m_fpsInterval(printInterval)
    , m_fpsTimePassed(0.0f)
    , m_framesPassed(0)
{
}

void FpsCounter::update(float delta)
{
    ++m_framesPassed;
    m_fpsTimePassed += delta;

    if (m_fpsTimePassed > m_fpsInterval) {
        std::cout << "fps: " << m_framesPassed / m_fpsTimePassed << std::endl;
        m_framesPassed = 0;
        m_fpsTimePassed -= m_fpsInterval;
    }
}
