#include "artemis/World.h"
#include "artemis/Manager.h"
#include "artemis/ComponentManager.h"
#include "artemis/EntityManager.h"
#include "artemis/EntitySystem.h"

namespace artemis
{

World::World()
{
   mCM = new ComponentManager();
   setManager<ComponentManager>(mCM);

   mEM = new EntityManager();
   setManager<EntityManager>(mEM);
}

World::~World()
{
   //deleteManager(mCM);
   //deleteManager(mEM);
   for (size_t i = 0; i < managersBag.size(); ++i) {
      Manager *m = managersBag.get(i);
      if (m)
         deleteManager(m);
   }
   managersBag.clear();
   for (size_t i = 0; i < systemsBag.size(); ++i) {
      EntitySystem *s = systemsBag.get(i);
      if (s)
         deleteSystem(s);
   }
   systemsBag.clear();
}

void World::initialize()
{
   for (size_t i = 0; i < managersBag.size(); ++i) {
      managersBag.get(i)->initialize();
   }

   for (size_t i = 0; i < systemsBag.size(); ++i) {
      EntitySystem *system = systemsBag.get(i);
      auto v = system->getMappers();
      for (size_t m_i = 0; m_i < v->size(); ++m_i) {
         v->get(m_i)->init(this);
      }
      if (system)
         systemsBag.get(i)->initialize();
   }
}

void World::deleteManager(Manager *manager)
{
   managersBag.set(manager->getType(), nullptr);
   delete manager;
}

Entity * World::createEntity()
{
   return mEM->createEntityInstance();
}

Entity * World::getEntity( int entityId )
{
   return mEM->getEntity(entityId);
}

void World::deleteSystem(EntitySystem *system)
{
   systemsBag.set(system->getType(), nullptr);
   delete system;
}

void World::notifySystems(void (EntityObserver::* func)(Entity *), Entity *e)
{
   size_t s = systemsBag.size();
   for (size_t i = 0; i < s; ++i) {
      if (systemsBag.get(i))
         (systemsBag.get(i)->*func)(e);
   }
}

void World::notifyManagers(void (EntityObserver::* func)(Entity *), Entity *e)
{
   size_t s = managersBag.size();
   for (size_t i = 0; i < s; ++i) {
      if (managersBag.get(i))
         (managersBag.get(i)->*func)(e);
   }
}

void World::check(Bag<Entity *> *entities, void (EntityObserver::* func)(Entity *))
{
   if (!entities->isEmpty()) {
      for (size_t i = 0; i < entities->size(); ++i) {
         Entity *e = entities->get(i);
         notifyManagers(func, e);
         notifySystems(func, e);
      }
      entities->clear();
   }
}

void World::process()
{
   check(&mAddedEntities, &EntityObserver::added);
   check(&mChangedEntities, &EntityObserver::changed);
   check(&mDisabledEntities, &EntityObserver::disabled);
   check(&mEnabledEntities, &EntityObserver::enabled);
   check(&mDeletedEntities, &EntityObserver::deleted);

   mCM->clean();
   mEM->clean();

   size_t s = systemsBag.size();
   for (size_t i = 0; i < s; ++i) {
      EntitySystem *system = systemsBag.get(i);
      if (system && !system->isPassive()) {
         system->process();
      }
   }
}

}
