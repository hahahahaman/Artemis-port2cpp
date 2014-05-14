#include "artemis/systems/DelayedEntityProcessingSystem.h"
#include "artemis/World.h"

namespace artemis
{

void DelayedEntityProcessingSystem::processEntities(Bag<Entity *> *entities)
{
   for (size_t i = 0, s = entities->size(); i < s; ++i) {
      Entity *entity = entities->get(i);
      processDelta(entity, acc);
      float remaining = getRemainingDelay(entity);
      if (remaining <= 0) {
         processExpired(entity);
      }
      else {
         offerDelay(remaining);
      }
   }
   stop();
}

void DelayedEntityProcessingSystem::inserted(Entity *e)
{
   float delay = getRemainingDelay(e);
   if (delay > 0) {
      offerDelay(delay);
   }
}

bool DelayedEntityProcessingSystem::checkProcessing()
{
   if (running) {
      acc += getWorld()->getDelta();

      if (acc >= delay) {
         return true;
      }
   }
   return false;
}

}