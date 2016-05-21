/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HEROCONTROLLER_H
#define HEROCONTROLLER_H

#include "KeyboardHandler.h"
#include "actors/Actor.h"

class HeroController : public KeyboardHandler
{
public:
    HeroController();

    void setActor(ActorId actorId);

    // KeyboardHandler interface
    bool onKeyDown(const unsigned int kcode);
    bool onKeyUp(const unsigned int kcode);

private:
    ActorId m_actorId;
};

#endif // HEROCONTROLLER_H
