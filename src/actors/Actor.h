#ifndef ACTOR_H
#define ACTOR_H

#include "ActorCategory.h"
#include "components/ActorComponent.h"

#include <map>
#include <memory>
#include <string>

class GameLogic;

using ActorId           = unsigned long;
using ActorPtr          = std::shared_ptr<Actor>;
using ActorComponentPtr = std::shared_ptr<ActorComponent>;

/*!
 * \brief Game entity.
 *
 * Represents game objects like NPCs, powerups, bullets, static walls.
 */
class Actor final
{
    friend class ActorFactory;

    using ComponentsMap = std::map<ActorComponentId, ActorComponentPtr>;

  public:
    Actor(ActorId id, GameLogic* game);
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

  private:
    ActorId m_id;
    GameLogic* m_game; //!< Owner
    ActorCategory m_category;
    ComponentsMap m_components;
    bool m_dead; //!< Flag indicating that this actor should be deleted by
                 //! GameLogic
    std::string m_name; //!< Name used in debug

    // Should be called only by ActorFactory
    void addComponent(ActorComponentPtr c)
    {
        c->setOwner(this);
        m_components.insert(std::make_pair(c->componentId(), c));
    }
};

#endif // ACTOR_H
