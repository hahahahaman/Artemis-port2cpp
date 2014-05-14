#include "artemis/EntityManager.h"
#include "artemis/ManagerType.h"
#include "artemis/Entity.h"

namespace artemis
{

EntityManager::EntityManager() : Manager(mtEntityManager), mActiveCnt(0), mAddedCnt(0), mCreatedCnt(0), mDeletedCnt(0)
{
}

Entity * EntityManager::createEntityInstance()
{
   Entity *e = new Entity(world, identifierPool.checkOut());
   mCreatedCnt++;
   return e;
}

void EntityManager::added( Entity *e )
{
   mActiveCnt++;
   mAddedCnt++;
   mEntities.set(e->getId(), e);
}

void EntityManager::enabled( Entity *e )
{
   mDisabledEntities.reset(e->getId());
}

void EntityManager::disabled( Entity *e )
{
   mDisabledEntities.set(e->getId());
}

void EntityManager::deleted( Entity *e )
{
   mDisabledEntities.reset(e->getId());
   mDeletedEntities.set(e->getId());
   mActiveCnt--;
}

void EntityManager::clean()
{
   if (mDeletedEntities.any()) {
      for (size_t i = 0; i < mDeletedEntities.size(); ++i) {
         if (mDeletedEntities.test(i)) {
            Entity *ent = mEntities.get(i);
            mEntities.set(ent->getId(), nullptr);
            identifierPool.checkIn(ent->getId());
            delete ent;
            mDeletedCnt++;
         }
      }
   }
   mDeletedEntities.reset();
}

EntityManager::~EntityManager()
{
   for (size_t i = 0; i < mEntities.size(); ++i) {
      delete mEntities.get(i);
   }
}


int EntityManager::IdentifierPool::checkOut()
{
   if (mIds.size() > 0) {
      auto res = mIds.back();
      mIds.pop_back();
      return res;
   }
   return nextAvailableId++;
}

void EntityManager::IdentifierPool::checkIn( int id )
{
   mIds.push_back(id);
}

}