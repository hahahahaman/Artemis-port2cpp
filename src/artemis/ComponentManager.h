#ifndef Artemis_ComponentManager_h__
#define Artemis_ComponentManager_h__

#include "artemis/Manager.h"
#include "artemis/utils/Bag.h"
#include "artemis/ComponentType.h"
#include "artemis/Component.h"

namespace artemis
{
class Component;
class Entity;
/**
 * Component Manager.
 *
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
class ComponentManager : public Manager
{
    friend class Entity;
    friend class World;
private:
    Bag< Bag<Component *> *> componentsByType;
    Bag<Entity *> deletedEntities;

public:
    ComponentManager(): Manager(mtComponentManager) {}
    ~ComponentManager()
    {
        for (size_t i = 0; i < componentsByType.size(); ++i)
        {
            Bag<Component *> *p = componentsByType.get(i);
            if(p != nullptr)
            {
                for (size_t j = 0; j < p->size(); ++j)
                {
                    delete p->get(j);
                }
                delete componentsByType.get(i);
            }
        }
    }

protected:
    void initialize() override {}

private:
    void removeComponentsOfEntity(Entity *e);

protected:
    void addComponent(Entity *e, Component *component);

    void removeComponent(Entity *e, ComponentType componentType);

    Component * getComponent(Entity *e, ComponentType componentType);

public:
    Bag<Component *> * getComponentsByType(ComponentType componentType);
    Bag<Component *> * getComponentsFor(Entity *e, Bag<Component *> *fillBag);


    void deleted(Entity *e) override
    {
        deletedEntities.add(e);
    }

    void clean();

};
}
#endif // Artemis_ComponentManager_h__
