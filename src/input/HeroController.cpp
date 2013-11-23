/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#include "HeroController.h"

#include "events/EventMgr.h"
#include <SDL.h>

#define TEST 0

#if TEST
    #include <iostream>
#endif

using namespace event;

HeroController::HeroController()
    : m_actorId(0)
{
    // ctor code
}

void HeroController::setActor(ActorId actorId)
{
    m_actorId = actorId;
}

bool HeroController::onKeyDown(const unsigned int kcode)
{
#if TEST
    std::clog << "KeyDown:" << kcode;
#endif
    if (!m_actorId)
        return false;

    if (kcode == SDL_SCANCODE_RIGHT) {
        EventMgr::singleton().queueEvent(EventPtr(new ActorInputEvent(m_actorId, MOVE_RIGHT_START)));
    }

    if (kcode == SDL_SCANCODE_LEFT) {
        EventMgr::singleton().queueEvent(EventPtr(new ActorInputEvent(m_actorId, MOVE_LEFT_START)));
    }

    if (kcode == SDL_SCANCODE_DOWN) {
        EventMgr::singleton().queueEvent(EventPtr(new ActorInputEvent(m_actorId, MOVE_DOWN_START)));
    }

    if (kcode == SDL_SCANCODE_UP) {
        EventMgr::singleton().queueEvent(EventPtr(new ActorInputEvent(m_actorId, JUMP)));
    }

    return true;
}

bool HeroController::onKeyUp(const unsigned int kcode)
{
#if TEST
    std::clog << "KeyUp  :" << kcode;
#endif
    if (!m_actorId)
        return false;

    if (kcode == SDL_SCANCODE_RIGHT) {
        EventMgr::singleton().queueEvent(EventPtr(new ActorInputEvent(m_actorId, MOVE_RIGHT_END)));
    }

    if (kcode == SDL_SCANCODE_LEFT) {
        EventMgr::singleton().queueEvent(EventPtr(new ActorInputEvent(m_actorId, MOVE_LEFT_END)));
    }

    if (kcode == SDL_SCANCODE_DOWN) {
        EventMgr::singleton().queueEvent(EventPtr(new ActorInputEvent(m_actorId, MOVE_DOWN_END)));
    }

    return true;
}
