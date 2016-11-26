/* -*- c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 */
#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

class FpsCounter final
{
  public:
    explicit FpsCounter(float printInterval = 5.0f);

    void update(float delta);

  private:
    const float m_fpsInterval;
    float m_fpsTimePassed;
    unsigned int m_framesPassed;
};

#endif // FPSCOUNTER_H
