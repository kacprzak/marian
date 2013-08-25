/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef EVENTTYPE_H
#define EVENTTYPE_H

enum EventType {
    ACTOR_CREATED,
    ACTOR_DESTROYED,
    ACTOR_KILLED,
    ACTOR_MOVED,
    ACTOR_COLLIDED,
    ACTOR_PHYSICS_STATE_CHANGED,

    GAME_STARTED,
    GAME_FINISHED,

    INPUT_COMMAND
};

enum InputCommand {
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN,
    FIRE
};

#endif // EVENTTYPE
