/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_H
#define GAME_H

#include "BaseGameLogic.h"
#include "FpsCounter.h"

//#define PRINT_FPS

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

#ifdef PRINT_FPS
    FpsCounter m_fpsCounter;
#endif
};

#endif
