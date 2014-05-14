#include "artemis/ComponentManager.h"
#include "artemis/Entity.h"
#include "artemis/Component.h"

namespace artemis
{

void ComponentManager::removeComponentsOfEntity(Entity *e)
{
   std::bitset<64> &componentBits = e->getComponentBits();
   for (size_t i = 0; i < componentBits.size(); ++i) {
      removeComponent(e, i);
   }
   //componentBits.reset();
}

void ComponentManager::addComponent(Entity *e, Component *component)
{
   componentsByType.ensureCapacity(component->getType());

   Bag<Component*> *components = componentsByType.get(component->getType());
   if (components == nullptr) {
      components = new Bag<Component *>();
      componentsByType.set(component->getType(), components);
   }

   if (components->get(e->getId())) {
      removeComponent(e, component->getType()); // unusual situation, avoid it by logic
   }
   components->set(e->getId(), component);

   e->getComponentBits().set(component->getType());
}

Bag<Component *> * ComponentManager::getComponentsByType(ComponentType componentType)
{
   Bag<Component *> *components = componentsByType.get(componentType);
   if (components == nullptr) {
      components = new Bag<Component *>();
      componentsByType.set(componentType, components);
   }
   return components;
}

void ComponentManager::removeComponent(Entity *e, ComponentType componentType )
{
   if (e->getComponentBits().test(componentType)) {
      delete componentsByType.get(componentType)->get(e->getId());
      componentsByType.get(componentType)->set(e->getId(), nullptr);
      e->getComponentBits().reset(componentType);
   }
}

Component * ComponentManager::getComponent(Entity *e, ComponentType componentType)
{
   Bag<Component *> *components = componentsByType.get(componentType);
   if (components != nullptr) {
      return components->get(e->getId());
   }
   return nullptr;
}

Bag<Component *> * ComponentManager::getComponentsFor(Entity *e, Bag<Component *> *fillBag)
{
   std::bitset<64> &componentBits = e->getComponentBits();

   for (size_t i = 0; i < componentBits.size(); ++i) {
      if (componentBits.test(i)) {
         fillBag->add(componentsByType.get(i)->get(e->getId()));
      }
   }
   return fillBag;
}

void ComponentManager::clean()
{
   if (deletedEntities.size() > 0) {
      for (size_t i = 0; deletedEntities.size() > i; ++i) {
         removeComponentsOfEntity(deletedEntities.get(i));
      }
      deletedEntities.clear();
   }
}

}