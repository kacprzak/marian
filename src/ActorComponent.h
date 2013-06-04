/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef ACTORCOMPONENT_H
#define ACTORCOMPONENT_H

//#include "Actor.h"
//#include "ActorFactory.h"
#include "ActorComponentId.h"
#include <memory>

class Actor;
class Engine;

class ActorComponent
{
    friend class ActorFactory;

 public:
    virtual ~ActorComponent();

    virtual bool init();
    virtual void update(Engine *e, float elapsedTime);
  
    virtual ActorComponentId componentId() const = 0;

 private:
    // Only ActroFactory should use this
    void setOwner(std::shared_ptr<Actor> actor) {
        m_owner = actor;
    }

 protected:
    std::shared_ptr<Actor> m_owner;

};

#endif // ACTORCOMPONENT_H
