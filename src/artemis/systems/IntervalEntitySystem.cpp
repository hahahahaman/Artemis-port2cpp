#include "artemis/systems/IntervalEntitySystem.h"
#include "artemis/World.h"

namespace artemis
{

bool IntervalEntitySystem::checkProcessing()
{
   acc += getWorld()->getDelta();
   if (acc >= interval)
   {
        acc -= interval;
        return true;
   }
   return false;
}

}
