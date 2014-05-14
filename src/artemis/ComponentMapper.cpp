#include "artemis/ComponentMapper.h"
#include "artemis/World.h"
#include "artemis/ComponentManager.h"
#include "artemis/utils/Bag.h"

namespace artemis
{
Bag<Component *> * ComponentMapperHelper::getComponents( World *w, ComponentType tp )
{
   return w->getComponentManager()->getComponentsByType(tp);
}
}