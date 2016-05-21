/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "ActorComponent.h"

class AiComponent : public ActorComponent
{
    friend class ActorFactory;

 public:
    virtual ~AiComponent();

    /*!
     * \brief Handle contact with some other object.
     *
     * \param other      object that contacts this object
     * \param fixtureUD  user data of fixture that collided with other
     */
    virtual void handleBeginContact(Actor * /*other*/,
                                    void * /*fixtureUD*/ = nullptr) {}
    virtual void handleEndContact  (Actor * /*other*/,
                                    void * /*fixtureUD*/ = nullptr) {}


    ActorComponentId componentId() const { return AI; }
};

#endif // AICOMPONENT_H
