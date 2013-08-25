#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <boost/utility.hpp>
#include <memory>
#include <list>

#include "GameView.h"
#include "PhysicsEngine.h"

class Engine;

typedef std::list<std::shared_ptr<GameView> > GameViewList;

class GameLogic : boost::noncopyable
{
 public:
    virtual ~GameLogic() {}

    virtual void update(float elapsedTime) = 0;

    /**  Use these if you need Engine to create or clean stuff */
    virtual void initialize(Engine * /*e*/) {}
    virtual void cleanup(Engine * /*e*/) {}

    virtual PhysicsEngine *physicsEngine() = 0;

    GameViewList& gameViews() { return m_gameViews; }

    void drawDebugData() { physicsEngine()->drawDebugData(); }
    void toggleDrawDebug() { physicsEngine()->toggleDrawDebug(); }

    void attachView(std::shared_ptr<GameView> gameView)
    {
        m_gameViews.push_back(gameView);
    }

private:
    GameViewList m_gameViews;
};

#endif // GAMELOGIC_H
