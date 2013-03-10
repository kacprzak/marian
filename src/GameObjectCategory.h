/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef GAME_OBJECT_CATEGORY_H
#define GAME_OBJECT_CATEGORY_H

enum GameObjectCategory {
    GROUND = 0x0001,
    HERO   = 0x0002,
    BOX    = 0x0004,
    SENSOR = 0x0008
};

#endif // GAME_OBJECT_CATEGORY_H
