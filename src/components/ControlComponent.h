/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef CONTROLCOMPONENT_H
#define CONTROLCOMPONENT_H

#include "ActorComponent.h"

class ControlComponent : public ActorComponent
{
    friend class ControlFactory;

 public:
    ActorComponentId componentId() const { return CONTROL; }
};

#endif // CONTROLCOMPONENT_H
