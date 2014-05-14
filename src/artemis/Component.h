#ifndef Artemis_Component_h__
#define Artemis_Component_h__

#include "artemis/ComponentType.h"

/**
 * A tag class. All components in the system must extend this class.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 */
namespace artemis
{

class Component
{
private:
   const ComponentType mType; // TODO: move to static or template param
public:
   Component(ComponentType tp): mType(tp) {}
   ComponentType getType() const { return mType; }
   virtual ~Component() {};
};

}
#endif // Artemis_Component_h__