/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef PLAYABLE_H
#define PLAYABLE_H

#include <SDL.h>
#include <boost/utility.hpp>

class Engine;

class Playable : boost::noncopyable
{
 public:
    virtual ~Playable() {}

    /**  Use these if you need Engine to create or clean stuff */
    virtual void initialize(Engine * /*e*/) {};
    virtual void cleanup(Engine * /*e*/) {};

    virtual bool processInput(const SDL_Event& event) = 0;
    virtual void update(Engine *e, float elapsedTime) = 0;
    virtual void draw(Engine *e) = 0;
};

#endif
