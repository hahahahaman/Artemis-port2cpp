#include "artemis/Entity.h"
#include "artemis/Component.h"
#include "artemis/ComponentManager.h"
#include "artemis/EntityManager.h"
#include "artemis/World.h"

namespace artemis
{

Entity::Entity( World *world, int id ) : world(world), id(id)
{
   entityManager = world->getEntityManager();
   componentManager = world->getComponentManager();

   reset();
}

Entity * Entity::addComponent(Component *component) {
   componentManager->addComponent(this, component);
   return this;
}

Entity * Entity::removeComponent(ComponentType componentType)
{
   componentManager->removeComponent(this, componentType);
   return this;
}

Entity * Entity::removeComponent(Component *component)
{
   return removeComponent(component->getType());
}

bool Entity::isActive() const
{
   return entityManager->isActive(id);
}

bool Entity::isEnabled()
{
   return entityManager->isEnabled(id);
}

Component * Entity::getComponent(ComponentType componentType)
{
   return componentManager->getComponent(this, componentType);
}

Bag<Component *> * Entity::getComponents(Bag<Component *> *fillBag)
{
   return componentManager->getComponentsFor(this, fillBag);
}

void Entity::addToWorld()
{
   world->addEntity(this);
}

void Entity::changedInWorld()
{
   world->changedEntity(this);
}

void Entity::deleteFromWorld()
{
   world->deleteEntity(this);
}

void Entity::enable()
{
   world->enable(this);
}

void Entity::disable()
{
   world->disable(this);
}

}