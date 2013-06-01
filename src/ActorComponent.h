/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef ACTORCOMPONENT_H
#define ACTORCOMPONENT_H

#include "Actor.h"
#include "ActorFactory.h"
#include "ActorComponentId.h"

class ActorComponent
{
    friend class ActorFactory;

 public:
    virtual ~ActorComponent();

    virtual bool init();
    virtual void update();
  
    virtual ActorComponentId componentId() const = 0;

 private:
    // Only ActroFactory should use this
    void setOwner(ActorPtr actor) {
        m_owner = actor;
    }

 protected:
    ActorPtr m_owner;

};

#endif // ACTORCOMPONENT_H
