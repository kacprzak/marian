/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef HUMANVIEW_H
#define HUMANVIEW_H

#include "GameView.h"

struct ViewRect
{
    float left, right, top, bottom;
};

//------------------------------------------------------------------------------

class HumanView : public GameView
{
 public:
    HumanView(const std::string& title, int screenWidth, int screenHeight,
              bool screenFull = false);

    ~HumanView();

    // GameView interface
    bool processInput(const SDL_Event &event) override;
    void update(float elapsedTime) override;

    void centerViewOn(float x, float y);
    void viewBounds(ViewRect *rect);

    void setBackgroundColor(int r, int g, int b);
    void setBackgroundColor(const std::string& color);

    /** Screen size in pixels */
    int screenWidth() const  { return m_screenWidth; }
    int screenHeight() const { return m_screenHeight; }

 protected:
    void preDraw();
    void postDraw();

 private:
    void createSDLWindow();
    void initializeOpenGL();

    std::string m_titile;
    int m_screenWidth;
    int m_screenHeight;
    bool m_screenFull;

    SDL_Window *m_window;
    SDL_GLContext m_glContext;

    float m_translate_x;
    float m_translate_y;
    float m_translate_z;
    float m_scale;
};

#endif // HUMANVIEW_H
