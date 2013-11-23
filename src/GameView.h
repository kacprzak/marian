#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <boost/utility.hpp>
#include <SDL.h>

class GameView : private boost::noncopyable
{
 public:

    virtual ~GameView() {}

    virtual bool processInput(const SDL_Event& event) = 0;
    virtual void update(float elapsedTime) = 0;
    virtual void draw() = 0;

    /*! Callbacks */
    virtual void onAttach(int /*gameViewId*/, unsigned long /*actorId*/) {}
};

#endif // GAMEVIEW_H
