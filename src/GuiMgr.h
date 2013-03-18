/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GUIMGR_H
#define GUIMGR_H

#include <SDL.h>
#include "Singleton.h"

class GuiMgr : public Singleton<GuiMgr>
{
 public:
    GuiMgr();

    bool processInput(const SDL_Event& event);
    void update(float elapsedTime);
    void draw();

 private:
    void handle_mouse_up(Uint8 button);
    void handle_mouse_down(Uint8 button);

};

#endif // GUIMGR_H
