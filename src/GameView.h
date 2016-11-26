#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <SDL.h>
#include <boost/utility.hpp>

class GameView : private boost::noncopyable
{
  public:
    virtual ~GameView() {}

    virtual bool processInput(const SDL_Event& event) = 0;
    virtual void update(float elapsedTime)            = 0;
    virtual void draw()                               = 0;

    /*! Callbacks */
    virtual void onAttach(int /*gameViewId*/, unsigned long /*actorId*/) {}
};

#endif // GAMEVIEW_H
