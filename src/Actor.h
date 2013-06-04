/* -*- c-basic-offset: 4; indent-tabs-mode: nil; -*- */
#ifndef ACTOR_H
#define ACTOR_H

#include "ActorCategory.h"
#include <Box2D/Box2D.h>
#include <string>
#include <iostream>
//#include "ActorFactory.h"
#include "ActorComponent.h"
#include <map>
#include <memory>

class Game;
//class ActorComponent;

typedef unsigned long ActorId;
typedef std::shared_ptr<Actor> ActorPtr;
typedef std::shared_ptr<ActorComponent> ActorComponentPtr;

class Actor final
{
    friend class ActorFactory;

    typedef std::map<ActorComponentId, ActorComponentPtr> ComponentsMap;

 public:
    Actor(ActorId id, Game *game)
        : m_id(id)
        , m_game(game)
        , m_body(nullptr)
        , m_dead(false)
    {}

    ~Actor() {
#ifndef NDEBUG
        std::clog << "Actor removed: id = " << m_id << " name = " << name() << '\n';
#endif 
        if (m_body) {
            m_body->SetUserData(nullptr); // To silence contact listener
            m_body->GetWorld()->DestroyBody(m_body);
        }
    }

    ActorId id() const { return m_id; }

    ActorCategory category() const { return m_category; }
    void setCategory(ActorCategory c) { m_category = c; }

    void update(Engine *e, float elapsedTime) {
        for (const auto &pair : m_components)
            pair.second->update(e, elapsedTime);
    }

    b2Body *body() { return m_body; }
    void setBody(b2Body *body)
    {
        assert(m_body == nullptr);
        m_body = body;
    }

    void die() { m_dead = true; }
    bool dead() const { return m_dead; }

    void setName(const std::string& name) { m_name = name; }
    const std::string& name() const { return m_name; }

    template <class T> std::weak_ptr<T> getComponent(ActorComponentId id) {
        ComponentsMap::iterator found = m_components.find(id);
        if (found != m_components.end()) {
            ActorComponentPtr base(found->second);
            // Cast to subclass
            std::shared_ptr<T> sub(std::static_pointer_cast<T>(base));
            // Conver to weak_ptr
            std::weak_ptr<T> weak(sub);
            return weak;
        } else {
            return std::weak_ptr<T>();
        }
    }

 protected:
    ActorId     m_id;
    Game       *m_game;
    ActorCategory m_category; 
    ComponentsMap m_components;
    b2Body     *m_body;
    bool        m_dead;
    std::string m_name;

 private:
    // Sholud be called only by ActorFactory
    void addComponent(ActorComponentPtr c) {
        m_components.insert(std::make_pair(c->componentId(), c));
    }

};

#endif // ACTOR_H

