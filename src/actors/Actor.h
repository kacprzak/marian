#ifndef ACTOR_H
#define ACTOR_H

#include "ActorCategory.h"
#include "../components/ActorComponent.h"

#include <map>
#include <memory>
#include <string>

class GameLogic;

using ActorId = unsigned long;

/*!
 * \brief Game entity.
 *
 * Represents game objects like NPCs, powerups, bullets, static walls.
 */
class Actor final
{
    friend class ActorFactory;

    using ComponentsMap =
        std::map<ActorComponentId, std::shared_ptr<ActorComponent>>;

  public:
    Actor(ActorId id, GameLogic* game);
    Actor(const Actor&) = delete;
    Actor& operator=(const Actor&) = delete;
    ~Actor();

    ActorId id() const { return m_id; }

    ActorCategory category() const { return m_category; }
    void setCategory(ActorCategory c) { m_category = c; }

    void update(float elapsedTime)
    {
        for (const auto& pair : m_components)
            pair.second->update(elapsedTime);
    }

    void die();
    bool dead() const { return m_dead; }

    void setName(const std::string& name) { m_name = name; }
    const std::string& name() const { return m_name; }

    template <class T>
    std::weak_ptr<T> getComponent(ActorComponentId id)
    {
        ComponentsMap::iterator found = m_components.find(id);
        if (found != m_components.end()) {
            // Cast to subclass
            std::shared_ptr<T> sub{std::static_pointer_cast<T>(found->second)};
            // Conver to weak_ptr
            return std::weak_ptr<T>{sub};
        } else {
            return std::weak_ptr<T>{};
        }
    }

  private:
    ActorId m_id;
    GameLogic* m_game; //!< Owner
    ActorCategory m_category;
    ComponentsMap m_components;
    bool m_dead; //!< Flag indicating that this actor should be deleted by
                 //! GameLogic
    std::string m_name; //!< Name used in debug

    // Should be called only by ActorFactory
    void addComponent(const std::shared_ptr<ActorComponent>& c)
    {
        c->setOwner(this);
        m_components.insert(std::make_pair(c->componentId(), c));
    }
};

#endif // ACTOR_H
