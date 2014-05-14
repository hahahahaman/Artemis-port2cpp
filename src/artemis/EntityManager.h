#ifndef Artemis_EntityManager_h__
#define Artemis_EntityManager_h__

#include "artemis/Manager.h"
#include "artemis/utils/Bag.h"
#include <bitset>
#include <vector>

namespace artemis
{
/**
 * Entity Manager.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
class EntityManager : public Manager
{
   friend class World;
private:
   /*
	 * Used only internally to generate distinct ids for entities and reuse them.
	 */
   class IdentifierPool {
   private:
      std::vector<unsigned int> mIds;
		unsigned int nextAvailableId;

   public:
      IdentifierPool(): nextAvailableId(0)
      {
         mIds.reserve(64);
      }
      ~IdentifierPool() {}
		
		int checkOut();
		void checkIn(int id);
	};

   Bag<Entity *> mEntities;
	std::bitset<64> mDisabledEntities;
   std::bitset<64> mDeletedEntities;
	
	int mActiveCnt;
	long int mAddedCnt;
	long int mCreatedCnt;
	long int mDeletedCnt;

	IdentifierPool identifierPool;
	
public:
   EntityManager();
   ~EntityManager();
	
protected:
   void initialize() override {}
	Entity * createEntityInstance();
	
public:
   void added(Entity *e) override;
	void enabled(Entity *e) override;
	void disabled(Entity *e) override;
	void deleted(Entity *e) override;


	/**
	 * Check if this entity is active.
	 * Active means the entity is being actively processed.
	 * 
	 * @param entityId
	 * @return true if active, false if not.
	 */
	bool isActive(int entityId) {
		return mEntities.get(entityId) != nullptr;
	}
	
	/**
	 * Check if the specified entityId is enabled.
	 * 
	 * @param entityId
	 * @return true if the entity is enabled, false if it is disabled.
	 */
	bool isEnabled(int entityId) {
		return !mDisabledEntities.test(entityId);
	}
	
	/**
	 * Get a entity with this id.
	 * 
	 * @param entityId
	 * @return the entity
	 */
protected:
   Entity * getEntity(int entityId) {
		return mEntities.get(entityId);
	}
	
	/**
	 * Get how many entities are active in this world.
	 * @return how many entities are currently active.
	 */
public:
   int getActiveEntityCount() { return mActiveCnt; }
	
	/**
	 * Get how many entities have been created in the world since start.
	 * Note: A created entity may not have been added to the world, thus
	 * created count is always equal or larger than added count.
	 * @return how many entities have been created since start.
	 */
	long int getTotalCreated() { return mCreatedCnt; }
	
	/**
	 * Get how many entities have been added to the world since start.
	 * @return how many entities have been added.
	 */
	long int getTotalAdded() { return mAddedCnt; }
	
	/**
	 * Get how many entities have been deleted from the world since start.
	 * @return how many entities have been deleted since start.
	 */
	long int getTotalDeleted() { return mDeletedCnt; }

   void clean();
};
}
#endif // Artemis_EntityManager_h__
