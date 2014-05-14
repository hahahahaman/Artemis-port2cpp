#ifndef Artemis_World_h__
#define Artemis_World_h__

#include "artemis/ManagerType.h"
#include "artemis/EntitySystemType.h"
#include "artemis/ComponentType.h"
#include "artemis/utils/Bag.h"
#include "artemis/ComponentMapper.h"
#include <map>

namespace artemis
{
class Manager;
class Entity;
class EntityManager;
class ComponentManager;
class EntitySystem;
class EntityObserver;
/**
 * The primary instance for the framework. It contains all the managers.
 *
 * You must use this to create, delete and retrieve entities.
 *
 * It is also important to set the delta each game loop iteration, and initialize before game loop.
 *
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
class World
{
private:
   EntityManager *mEM;
	ComponentManager *mCM;

public:
   float delta;

private:
   Bag<Entity *> mAddedEntities;
	Bag<Entity *> mChangedEntities;
	Bag<Entity *> mDeletedEntities;
	Bag<Entity *> mEnabledEntities;
	Bag<Entity *> mDisabledEntities;

	Bag<Manager *> managersBag;
	Bag<EntitySystem *> systemsBag;

public:
   World();
   ~World();

	/**
	 * Makes sure all managers systems are initialized in the order they were added.
	 */
	void initialize();

	/**
	 * Returns a manager that takes care of all the entities in the world.
	 * entities of this world.
	 *
	 * @return entity manager.
	 */
	EntityManager * getEntityManager() { return mEM; }

	/**
	 * Returns a manager that takes care of all the components in the world.
	 *
	 * @return component manager.
	 */
	ComponentManager * getComponentManager() {return mCM; }

   /**
	 * Add a manager into this world. It can be retrieved later.
	 * World will notify this manager of changes to entity.
	 *
	 * @param manager to be added
	 */
   template<typename T>
	T * setManager(T *manager)
   {
		managersBag.set(manager->getType(), manager);
		manager->setWorld(this);
		return manager;
	}

	/**
	 * Returns a manager of the specified type.
	 *
	 * @param <T>
	 * @param managerType
	 *            class type of the manager
	 * @return the manager
	 */
   template<typename T>
	T * getManager(ManagerType tp)
   {
		return static_cast<T*>(managersBag.get(tp));
	}

	/**
	 * Deletes the manager from this world.
	 * @param manager to delete.
	 */
	void deleteManager(Manager *manager);

	/**
	 * Time since last game loop.
	 *
	 * @return delta time since last game loop.
	 */
	float getDelta() const { return delta; }

	/**
	 * You must specify the delta for the game here.
	 *
	 * @param delta time since last game loop.
	 */
	void setDelta(float dt) { this->delta = dt; }

	/**
	 * Adds a entity to this world.
	 *
	 * @param e entity
	 */
	void addEntity(Entity *e) { mAddedEntities.add(e); }

	/**
	 * Ensure all systems are notified of changes to this entity.
	 * If you're adding a component to an entity after it's been
	 * added to the world, then you need to invoke this method.
	 *
	 * @param e entity
	 */
	void changedEntity(Entity *e) { mChangedEntities.add(e);
	}

	/**
	 * Delete the entity from the world.
	 *
	 * @param e entity
	 */
	void deleteEntity(Entity *e)
   {
		if (!mDeletedEntities.contains(e)) {
			mDeletedEntities.add(e);
		}
	}

	/**
	 * (Re)enable the entity in the world, after it having being disabled.
	 * Won't do anything unless it was already disabled.
	 */
	void enable(Entity *e) { mEnabledEntities.add(e); }

	/**
	 * Disable the entity from being processed. Won't delete it, it will
	 * continue to exist but won't get processed.
	 */
	void disable(Entity *e) { mDisabledEntities.add(e); }

	/**
	 * Create and return a new or reused entity instance.
	 * Will NOT add the entity to the world, use World.addEntity(Entity) for that.
	 *
	 * @return entity
	 */
	Entity * createEntity();

	/**
	 * Get a entity having the specified id.
	 *
	 * @param entityId
	 * @return entity
	 */
	Entity * getEntity(int entityId);

	/**
	 * Gives you all the systems in this world for possible iteration.
	 *
	 * @return all entity systems in world.
	 */
	Bag<EntitySystem *> & getSystems() { return systemsBag; }

	/**
	 * Adds a system to this world that will be processed by World.process()
	 *
	 * @param system the system to add.
	 * @return the added system.
	 */
   template<typename T>
	T * setSystem(T *system) { return setSystem(system, false); }

	/**
	 * Will add a system to this world.
	 *
	 * @param system the system to add.
	 * @param passive wether or not this system will be processed by World.process()
	 * @return the added system.
	 */
	template<typename T>
   T * setSystem(T *system, bool passive)
   {
		system->setWorld(this);
		system->setPassive(passive);
		systemsBag.set(system->getType(), system);

		return system;
	}

	/**
	 * Removed the specified system from the world.
	 * @param system to be deleted from world.
	 */
	void deleteSystem(EntitySystem *system);

private:
   void notifySystems(void (EntityObserver::* func)(Entity *), Entity *e);
   void notifyManagers(void (EntityObserver::* func)(Entity *), Entity *e);

public:
   /**
	 * Retrieve a system for specified system type.
	 *
	 * @param type type of system.
	 * @return instance of the system in this world.
	 */
   template<typename T>
   T * getSystem(EntitySystemType tp) {
		return static_cast<T *>(systemsBag.get(tp));
	}

private:
	/**
	 * Performs an action on each entity.
	 * @param entities
	 * @param performer
	 */
   void check(Bag<Entity *> *entities, void (EntityObserver::* func)(Entity *));

public:
	/**
	 * Process all non-passive systems.
	 */
   void process();

	/**
	 * Retrieves a ComponentMapper instance for fast retrieval of components from entities.
	 *
	 * @param type of component to get mapper for.
	 * @return mapper for specified component type.
	 */
   template<typename T, ComponentType cType>
	ComponentMapper<T, cType> * getMapper() {
		return ComponentMapper<T, cType>::getFor(this);
	}
};
}
#endif // Artemis_World_h__
