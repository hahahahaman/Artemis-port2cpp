#ifndef Artemis_EntitySystem_h__
#define Artemis_EntitySystem_h__

#include "artemis/EntitySystemType.h"
#include "artemis/EntityObserver.h"
#include "artemis/utils/Bag.h"
#include <bitset>
#include <vector>

namespace artemis
{
class Aspect;
class World;
class BaseComponentMapper;

/**
 * The most raw entity system. It should not typically be used, but you can create your own
 * entity system handling by extending this. It is recommended that you use the other provided
 * entity system implementations.
 *
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
class EntitySystem : public EntityObserver
{
   friend class World;
private:
   World *world;
   EntitySystemType mType;
   Bag<Entity *> mActives;
   Bag<BaseComponentMapper *> mRegisteredMappers;

	std::bitset<64> allSet;
	std::bitset<64> exclusionSet;
	std::bitset<64> oneSet;

	bool passive;
	bool dummy;

public:
   /**
	 * Creates an entity system that uses the specified aspect as a matcher against entities.
	 * @param aspect to match against entities
	 */
   EntitySystem(Aspect *pAspect, EntitySystemType tp);
   ~EntitySystem();
	EntitySystemType getType() const { return mType; }

   void process();

protected:
   void registerMapper(BaseComponentMapper *mp);
   Bag<BaseComponentMapper *> * getMappers();
  	/**
	 * Called before processing of entities begins.
	 */
   virtual void begin() {}
	/**
	 * Called after the processing of entities ends.
	 */
   virtual void end() {}

	/**
	 * Any implementing entity system must implement this method and the logic
	 * to process the given entities of the system.
	 *
	 * @param entities the entities this system contains.
	 */
   virtual void processEntities(Bag<Entity *> *entities) = 0;

	/**
	 *
	 * @return true if the system should be processed, false if not.
	 */
   virtual bool checkProcessing() = 0;

	/**
	 * Override to implement code that gets executed when systems are initialized.
	 */
	virtual void initialize() {};

	/**
	 * Called if the system has received a entity it is interested in, e.g. created or a component was added to it.
	 * @param e the entity that was added to this system.
	 */
   virtual void inserted(Entity *e) {};

	/**
	 * Called if a entity was removed from this system, e.g. deleted or had one of it's components removed.
	 * @param e the entity that was removed from this system.
	 */
	virtual void removed(Entity *e) {};

	/**
	 * Will check if the entity is of interest to this system.
	 * @param e entity to check
	 */
	void check(Entity *e);

private:
   void removeFromSystem(Entity *e);
	void insertToSystem(Entity *e);

public:
   void added(Entity *e) override;
	void changed(Entity *e) override;
	void deleted(Entity *e) override;
	void disabled(Entity *e) override;
	void enabled(Entity *e) override;

protected:
   void setWorld(World *world) { this->world = world; }
   World * getWorld() { return world; }
   bool isPassive() const { return passive; }
	void setPassive(bool passive) { this->passive = passive; }
public:
   const Bag<Entity *> * getActives() const { return &mActives; }
};
}
#endif // Artemis_EntitySystem_h__
