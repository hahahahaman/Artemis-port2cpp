#ifndef Artemis_Entity_h__
#define Artemis_Entity_h__

#include "artemis/utils/Bag.h"
#ifdef USE_BOOST_UUID
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#endif // USE_BOOST_UUID
#include "artemis/ComponentType.h"
#include <bitset>

namespace artemis
{
class Component;
class World;
class EntityManager;
class ComponentManager;

/**
 * The entity class. Cannot be instantiated outside the framework, you must
 * create new entities using World.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 * 
 */
class Entity 
{
   friend class EntityManager;
private:
#ifdef USE_BOOST_UUID
   boost::uuids::uuid uuid;
#endif // USE_BOOST_UUID

	int id;
	std::bitset<64> componentBits;
	std::bitset<64> systemBits;

	World *world;
	EntityManager *entityManager;
	ComponentManager *componentManager;
	
protected:
   Entity(World *world, int id);

	/**
	 * The internal id for this entity within the framework. No other entity
	 * will have the same ID, but ID's are however reused so another entity may
	 * acquire this ID if the previous entity was deleted.
	 * 
	 * @return id of the entity.
	 */
public:
   int getId() const { return id; }

	/**
	 * Returns a BitSet instance containing bits of the components the entity possesses.
	 * @return
	 */
   std::bitset<64>& getComponentBits() { return componentBits; }
	
	/**
	 * Returns a BitSet instance containing bits of the components the entity possesses.
	 * @return
	 */
	std::bitset<64>& getSystemBits() { return systemBits; }

	/**
	 * Make entity ready for re-use.
	 * Will generate a new uuid for the entity.
	 */
protected:
	void reset()
   {
		systemBits.reset();
		componentBits.reset();
#ifdef USE_BOOST_UUID
		uuid = boost::uuids::random_generator()();
#endif // USE_BOOST_UUID
	}

public:
   //std::string toString() override { return "Entity[" + id + "]"; }

	/**
	 * Add a component to this entity.
	 * 
	 * @param component to add to this entity
	 * 
	 * @return this entity for chaining.
	 */
	Entity * addComponent(Component *component);
	
	/**
	 * Faster adding of components into the entity. Not neccessery to use this, but
	 * in some cases you might need the extra performance.
	 * 
	 * @param component the component to add
	 * @param type of the component
	 * 
	 * @return this entity for chaining.
	 */
	//Entity * addComponent(Component *component, ComponentType componentType);

	/**
	 * Removes the component from this entity.
	 * 
	 * @param component to remove from this entity.
	 * 
	 * @return this entity for chaining.
	 */
	Entity * removeComponent(Component *component);

	/**
	 * Faster removal of components from a entity.
	 * 
	 * @param component to remove from this entity.
	 * 
	 * @return this entity for chaining.
	 */
// 	public Entity removeComponent(ComponentType type) {
// 		componentManager.removeComponent(this, type);
// 		return this;
// 	}
	
	/**
	 * Remove component by its type.
	 * @param type
	 * 
	 * @return this entity for chaining.
	 */
	Entity * removeComponent(ComponentType componentType);

	/**
	 * Checks if the entity has been added to the world and has not been deleted from it.
	 * If the entity has been disabled this will still return true.
	 * 
	 * @return if it's active.
	 */
	bool isActive() const;
	
	/**
	 * Will check if the entity is enabled in the world.
	 * By default all entities that are added to world are enabled,
	 * this will only return false if an entity has been explicitly disabled.
	 * 
	 * @return if it's enabled
	 */
	bool isEnabled();
	
	/**
	 * This is the preferred method to use when retrieving a component from a
	 * entity. It will provide good performance.
	 * But the recommended way to retrieve components from an entity is using
	 * the ComponentMapper.
	 * 
	 * @param type
	 *            in order to retrieve the component fast you must provide a
	 *            ComponentType instance for the expected component.
	 * @return
	 */
	Component * getComponent(ComponentType componentType);

	/**
	 * Slower retrieval of components from this entity. Minimize usage of this,
	 * but is fine to use e.g. when creating new entities and setting data in
	 * components.
	 * 
	 * @param <T>
	 *            the expected return component type.
	 * @param type
	 *            the expected return component type.
	 * @return component that matches, or null if none is found.
	 */
   template<typename T>
	T * getComponent(ComponentType tp) {
		return static_cast<T *>(getComponent(tp));
	}

	/**
	 * Returns a bag of all components this entity has.
	 * You need to reset the bag yourself if you intend to fill it more than once.
	 * 
	 * @param fillBag the bag to put the components into.
	 * @return the fillBag with the components in.
	 */
	Bag<Component *> * getComponents(Bag<Component *> *fillBag);

	/**
	 * Refresh all changes to components for this entity. After adding or
	 * removing components, you must call this method. It will update all
	 * relevant systems. It is typical to call this after adding components to a
	 * newly created entity.
	 */
	void addToWorld();
	
	/**
	 * This entity has changed, a component added or deleted.
	 */
	void changedInWorld();

	/**
	 * Delete this entity from the world.
	 */
	void deleteFromWorld();
	
	/**
	 * (Re)enable the entity in the world, after it having being disabled.
	 * Won't do anything unless it was already disabled.
	 */
	void enable();
	
	/**
	 * Disable the entity from being processed. Won't delete it, it will
	 * continue to exist but won't get processed.
	 */
	void disable();
	
#ifdef USE_BOOST_UUID
	/**
	 * Get the UUID for this entity.
	 * This UUID is unique per entity (re-used entities get a new UUID).
	 * @return uuid instance for this entity.
	 */
   const boost::uuids::uuid & getUuid() const { return uuid; }
#endif // USE_BOOST_UUID

	/**
	 * Returns the world this entity belongs to.
	 * @return world of entity.
	 */
	World *getWorld() { return world; }
};
}
#endif // Artemis_Entity_h__
