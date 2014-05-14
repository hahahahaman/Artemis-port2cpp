#include "artemis/EntitySystem.h"
#include "artemis/Aspect.h"
#include "artemis/Entity.h"

namespace artemis
{

EntitySystem::EntitySystem(Aspect *pAspect, EntitySystemType tp): mType(tp)
{
   allSet = pAspect->getAllSet();
   exclusionSet = pAspect->getExclusionSet();
   oneSet = pAspect->getOneSet();
   dummy = allSet.none() && oneSet.none(); // This system can't possibly be interested in any entity, so it must be "dummy"
   delete pAspect;
}

EntitySystem::~EntitySystem()
{
}

void EntitySystem::check(Entity *e)
{
   if (dummy) {
      return;
   }

   bool contains = e->getSystemBits().test(mType);
   bool interested = true; // possibly interested, let's try to prove it wrong.

   std::bitset<64> &componentBits = e->getComponentBits();

   // Check if the entity possesses ALL of the components defined in the aspect.
   if (!allSet.none()) {
      if ((allSet & componentBits) != allSet) {
         interested = false;
      }
      /*for (size_t i = 0; i < allSet.count(); ++i) {
         if (allSet.test(i) && !componentBits.test(i)) {
            interested = false;
            break;
         }
      }*/
   }

   // Check if the entity possesses ANY of the exclusion components, if it does then the system is not interested.
   if (!exclusionSet.none() && interested) {
      interested = !((exclusionSet & componentBits).any());
   }

   // Check if the entity possesses ANY of the components in the oneSet. If so, the system is interested.
   if (!oneSet.none()) {
      interested = (oneSet & componentBits).any();
   }

   if (interested && !contains) {
      insertToSystem(e);
   } else if (!interested && contains) {
      removeFromSystem(e);
   }
}

void EntitySystem::removeFromSystem(Entity *e)
{
   mActives.remove(e);
   e->getSystemBits().reset(mType);
   removed(e);
}

void EntitySystem::insertToSystem(Entity *e)
{
   mActives.add(e);
   e->getSystemBits().set(mType);
   inserted(e);
}

void EntitySystem::deleted(Entity *e)
{
   if (e->getSystemBits().test(mType)) {
      removeFromSystem(e);
   }
}

void EntitySystem::added( Entity *e )
{
   check(e);
}

void EntitySystem::changed( Entity *e )
{
   check(e);
}

void EntitySystem::disabled( Entity *e )
{
   if (e->getSystemBits().test(mType)) {
      removeFromSystem(e);
   }
}

void EntitySystem::enabled( Entity *e )
{
   check(e);
}

void EntitySystem::process()
{
   if (checkProcessing()) {
      begin();
      processEntities(&mActives);
      end();
   }
}

void EntitySystem::registerMapper( BaseComponentMapper *mp )
{
   mRegisteredMappers.add(mp);
}

Bag<BaseComponentMapper *> * EntitySystem::getMappers()
{
   return &mRegisteredMappers;
}

}