/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef IMAGERENDERCOMPONENT_H
#define IMAGERENDERCOMPONENT_H

#include "RenderComponent.h"
#include "graphics/Image.h"
#include <memory>
#include "Actor.h"
#include "Engine.h"

class ImageRenderComponent : public RenderComponent
{
    friend class ActorFactory;

 public:

    const Image& currentImage() const override {
        return *m_image;
    }

 protected:
    std::shared_ptr<Image> m_image;
};

#endif // IMAGERENDERCOMPONENT_H
