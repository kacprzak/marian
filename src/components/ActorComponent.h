#ifndef ACTORCOMPONENT_H
#define ACTORCOMPONENT_H

#include "ActorComponentId.h"

#include <memory>

class Actor;
class Engine;

class ActorComponent
{
    friend class Actor;

  public:
    virtual ~ActorComponent() {}

    virtual bool init() { return true; }
    virtual void update(float /*elapsedTime*/) {}

    virtual ActorComponentId componentId() const = 0;

  private:
    // Only Actor should use this
    void setOwner(Actor* actor) { m_owner = actor; }

  protected:
    Actor* m_owner = nullptr;
};

#endif // ACTORCOMPONENT_H
