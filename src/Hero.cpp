/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "Hero.h"

#include "Engine.h"
#include <iostream>

void Hero::update(Engine *e, float /*elapsedTime*/)
{
#if 0
    //static float v = 250.0f;
    static float frictionFactor = 1.2f;

    if (e->isPressed(SDLK_RIGHT)) {
        if (m_vx < 0.0f)
            m_vx /= frictionFactor;
        m_ax = 1000.0f;
    } else if (e->isPressed(SDLK_LEFT)) {
        if (m_vx > 0.0f)
            m_vx /= frictionFactor;
        m_ax = -1000.0f;
    } else {
        m_vx /= frictionFactor;
    }
#endif
    // Center view on player
    e->centerViewOn(position().x, position().y);
}

//------------------------------------------------------------------------------
