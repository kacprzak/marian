#ifndef SINGLETON_H
#define SINGLETON_H

#include <cassert>
#include <cstdint> // intptr_t

/*!
 * \brief Singleton pattern implementation with controll over creation
 * and destruction times.
 *
 * Based on article form "Game Programming Gems vol.1"
 */
template <typename T>
class Singleton
{
    static T* s_singleton;

  public:
    Singleton()
    {
        assert(!s_singleton);
        // Allows multiple inheritance
        T* imgObj          = (T*)(void*)1;
        Singleton<T>* imgS = (Singleton<T>*)imgObj;
        intptr_t offset    = (intptr_t)imgObj - (intptr_t)imgS;
        s_singleton        = (T*)((intptr_t) this + offset);
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    virtual ~Singleton()
    {
        assert(s_singleton);
        s_singleton = nullptr;
    }

    static T& singleton()
    {
        assert(s_singleton);
        return *s_singleton;
    }

    static T* singletonPtr() { return s_singleton; }
};

template <typename T>
T* Singleton<T>::s_singleton = nullptr;

#endif // SINGLETON_H
