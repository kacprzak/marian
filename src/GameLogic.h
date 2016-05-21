/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "GameView.h"
#include "PhysicsEngine.h"

#include <boost/utility.hpp>
#include <memory>
#include <list>

class Engine;

using GameViewList = std::list<std::shared_ptr<GameView> >;

class GameLogic : private boost::noncopyable
{
 public:
    virtual ~GameLogic() {}

    virtual void update(float elapsedTime) = 0;

    /*!  Use these if you need Engine to create or clean stuff */
    virtual void onBeforeMainLoop(Engine * /*e*/) {}
    virtual void onAfterMainLoop(Engine * /*e*/) {}

    virtual PhysicsEngine *physicsEngine() { return nullptr; }

    GameViewList& gameViews() { return m_gameViews; }

    void drawDebugData()
    {
      if (physicsEngine())
          physicsEngine()->drawDebugData();
    }

    void toggleDrawDebug()
    {
      if (physicsEngine())
          physicsEngine()->toggleDrawDebug();
    }

    void attachView(std::shared_ptr<GameView> gameView, ActorId actorId = 0)
    {
        int viewId = m_gameViews.size();
        m_gameViews.push_back(gameView);
        gameView->onAttach(viewId, actorId);
    }

 private:
    GameViewList m_gameViews;
};

#endif // GAMELOGIC_H
