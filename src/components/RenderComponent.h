/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "ActorComponent.h"
#include "Image.h"
#include "Event.h"

class RenderComponent : public ActorComponent
{
    friend class ActorFactory;

 public:
    //virtual void draw(Engine * /*e*/) = 0;

    virtual const Image& currentImage() const = 0;

    // Difference to physical transformation
    virtual float xOffset() const { return 0.0f; }
    virtual float yOffset() const { return 0.0f; }

    virtual void changePhysicsState(ActorPhysicsStateId /*newState*/) {}

    ActorComponentId componentId() const { return RENDER; }
};

#endif // RENDERCOMPONENT_H
