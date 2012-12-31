#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

class FpsCounter
{
public:
    FpsCounter(float printInterval = 5.0f);

    void update(float delta);

private:
    const float m_fpsInterval;
    float m_fpsTimePassed;
    unsigned int m_framesPassed;
};

#endif // FPSCOUNTER_H
