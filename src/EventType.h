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
    MOVE_RIGHT_START,
    MOVE_RIGHT_END,
    MOVE_LEFT_START,
    MOVE_LEFT_END,
    MOVE_DOWN_START,
    MOVE_DOWN_END,
    JUMP,
    FIRE
};

#endif // EVENTTYPE
