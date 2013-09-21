/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GUIMGR_H
#define GUIMGR_H

#include "Singleton.h"

#include <SDL.h>

class GuiMgr : public Singleton<GuiMgr>
{
 public:
    GuiMgr();
    ~GuiMgr();

    bool processInput(const SDL_Event& event);
    void update(float elapsedTime);
    void draw();

 private:
    void handle_mouse_up(Uint8 button);
    void handle_mouse_down(Uint8 button);
    void handle_mouse_wheel(const SDL_MouseWheelEvent& e);

};

#endif // GUIMGR_H
