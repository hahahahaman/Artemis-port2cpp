#ifndef Artemis_IntervalEntityProcessingSystem_h__
#define Artemis_IntervalEntityProcessingSystem_h__

#include "artemis/systems/IntervalEntitySystem.h"

namespace artemis
{

/**
 * If you need to process entities at a certain interval then use this.
 * A typical usage would be to regenerate ammo or health at certain intervals, no need
 * to do that every game loop, but perhaps every 100 ms. or every second.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
class IntervalEntityProcessingSystem : public IntervalEntitySystem
{

public:
   IntervalEntityProcessingSystem(Aspect *aspect, float interval, EntitySystemType tp): IntervalEntitySystem(aspect, interval, tp) {}

	/**
	 * Process a entity this system is interested in.
	 * @param e the entity to process.
	 */
protected:
   virtual void process(Entity *e) = 0;
	
	void processEntities(Bag<Entity *> *entities) override
   {
		for (size_t i = 0, s = entities->size(); i < s; ++i) {
			process(entities->get(i));
		}
	}

};
}
#endif // Artemis_IntervalEntityProcessingSystem_h__
