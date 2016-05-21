/* -*- c-file-style: "java"; c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef KEYBOARDHANDLER_H
#define KEYBOARDHANDLER_H

/*!
 * \brief Interface for handling keyboard input.
 */
class KeyboardHandler
{
    public:
    virtual ~KeyboardHandler() {}

    virtual bool onKeyDown(unsigned int const kcode) = 0;
    virtual bool onKeyUp(unsigned int const kcode) = 0;
};

#endif // KEYBOARDHANDLER_H
