/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "ActorComponent.h"

class RenderComponent : public ActorComponent
{
    friend class ActorFactory;

 public:
    virtual void draw(Engine * /*e*/) = 0;

    ActorComponentId componentId() const { return RENDER; }
};

#endif // RENDERCOMPONENT_H
