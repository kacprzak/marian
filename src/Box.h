/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef BOX_H
#define BOX_H

#include "Actor.h"

#include "Map.h"
#include "Image.h"
#include <memory>

class Box : public Actor
{
 public:
    Box(unsigned long id,
        Game *game,
        float x, float y,
        float w = 1.0f, float h = 1.0f);

    ActorCategory category() override
    { return BOX; }

    void draw(Engine *e) override;

 private:
    std::unique_ptr<Image> m_image;
};

#endif
