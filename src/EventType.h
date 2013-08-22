/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef EVENTTYPE_H
#define EVENTTYPE_H

enum EventType {
    ACTOR_CREATED,
    ACTOR_DESTROYED,
    ACTOR_KILLED,
    ACTOR_MOVED,
    ACTOR_COLLIDED,
    GAME_STARTED,
    GAME_FINISHED
};

#endif // EVENTTYPE