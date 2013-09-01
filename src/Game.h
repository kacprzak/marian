/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_H
#define GAME_H

#include "BaseGameLogic.h"
#include <SDL.h>
#include "FpsCounter.h"

class Game : public BaseGameLogic
{
 public:
    Game();

    void initialize(Engine *e);

    void update(float elapsedTime) override;

 private:

    void handleActorCollided(EventPtr event);
    //void handleActorPhysicsStateChanged(EventPtr event);
    void handleInputCommand(EventPtr event);

    EventListenerHelper elh;
    FpsCounter m_fpsCounter;
};

#endif
