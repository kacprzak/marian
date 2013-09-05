#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <boost/utility.hpp>
#include <SDL.h>

class Engine;

class GameView : boost::noncopyable
{
 public:

    virtual ~GameView() {}

    virtual bool processInput(const SDL_Event& event) = 0;
    virtual void update(float elapsedTime) = 0;
    virtual void draw(Engine *e) = 0;

    /**  Use these if you need Engine to create or clean stuff */
    virtual void initialize(Engine * /*e*/) {}
    virtual void cleanup(Engine * /*e*/) {}

    /** Callbacks */
    virtual void onAttach(int /*gameViewId*/, unsigned long /*actorId*/) {}
};

#endif // GAMEVIEW_H
