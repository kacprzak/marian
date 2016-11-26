/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 */
#ifndef GUIMGR_H
#define GUIMGR_H

#include "Singleton.h"

#include <SDL.h>

namespace gui {

class GuiMgr : public Singleton<GuiMgr>
{
  public:
    GuiMgr();
    ~GuiMgr() override;

    bool processInput(const SDL_Event& event);
    void update(float elapsedTime);
    void draw();

    void setViewSize(int width, int height);

  private:
    void handle_mouse_up(Uint8 button);
    void handle_mouse_down(Uint8 button);
    void handle_mouse_wheel(const SDL_MouseWheelEvent& e);
};

} // namespace gui

#endif // GUIMGR_H
