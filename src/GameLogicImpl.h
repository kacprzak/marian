/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 */
#ifndef GAME_H
#define GAME_H

#include "BaseGameLogic.h"

class GameLogicImpl : public BaseGameLogic
{
  public:
    GameLogicImpl();

    void onBeforeMainLoop(Engine *e) override;

    void update(float elapsedTime) override;

  private:
    void handleActorCollided(event::Event &event);
    // void handleActorPhysicsStateChanged(Event& event);
    void handleInputCommand(event::Event &event);

    event::EventListenerHelper elh;
};

#endif
