/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
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

    EventMgr& evtMgr = EventMgr::singleton();

    if (kcode == SDL_SCANCODE_RIGHT) {
        evtMgr.queueEvent(std::unique_ptr<Event>(new ActorInputEvent(m_actorId, MOVE_RIGHT_START)));
    }

    if (kcode == SDL_SCANCODE_LEFT) {
        evtMgr.queueEvent(std::unique_ptr<Event>(new ActorInputEvent(m_actorId, MOVE_LEFT_START)));
    }

    if (kcode == SDL_SCANCODE_DOWN) {
        evtMgr.queueEvent(std::unique_ptr<Event>(new ActorInputEvent(m_actorId, MOVE_DOWN_START)));
    }

    if (kcode == SDL_SCANCODE_UP) {
        evtMgr.queueEvent(std::unique_ptr<Event>(new ActorInputEvent(m_actorId, JUMP)));
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

    EventMgr& evtMgr = EventMgr::singleton();

    if (kcode == SDL_SCANCODE_RIGHT) {
        evtMgr.queueEvent(std::unique_ptr<Event>(new ActorInputEvent(m_actorId, MOVE_RIGHT_END)));
    }

    if (kcode == SDL_SCANCODE_LEFT) {
        evtMgr.queueEvent(std::unique_ptr<Event>(new ActorInputEvent(m_actorId, MOVE_LEFT_END)));
    }

    if (kcode == SDL_SCANCODE_DOWN) {
        evtMgr.queueEvent(std::unique_ptr<Event>(new ActorInputEvent(m_actorId, MOVE_DOWN_END)));
    }

    return true;
}
