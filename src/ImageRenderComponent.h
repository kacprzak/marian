/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef IMAGERENDERCOMPONENT_H
#define IMAGERENDERCOMPONENT_H

#include "RenderComponent.h"
#include "Image.h"
#include <memory>
#include "Actor.h"
#include "Engine.h"

class ImageRenderComponent : public RenderComponent
{
    friend class ActorFactory;

 public:
    void draw(Engine *e) override {
        // try to get Physics component
        // Get weak_ptr
        auto pcwp = m_owner->getComponent<PhysicsComponent>(PHYSICS);
        // Try to get shared_ptr 
        if (auto pcsp = pcwp.lock()) {
            e->drawImage(*m_image, pcsp->posX(), pcsp->posY(),
                         pcsp->angle());
        }

    }

 protected:
    std::shared_ptr<Image> m_image;
};

#endif // IMAGERENDERCOMPONENT_H
