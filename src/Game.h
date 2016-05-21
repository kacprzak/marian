/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_H
#define GAME_H

#include "BaseGameLogic.h"

class Game : public BaseGameLogic
{
    public:
    Game();

    void onBeforeMainLoop(Engine *e) override;

    void update(float elapsedTime) override;

    private:

    void handleActorCollided(event::Event& event);
    //void handleActorPhysicsStateChanged(Event& event);
    void handleInputCommand(event::Event& event);

    event::EventListenerHelper elh;
};

#endif
