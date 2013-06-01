/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "ActorComponent.h"

class RenderComponent : public ActorComponent
{
    friend class RenderFactory;

 public:
    virtual ~RenderComponent();

    ActorComponentId componentId() const { return RENDER; }
};

#endif // RENDERCOMPONENT_H
