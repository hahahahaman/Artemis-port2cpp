#ifndef Artemis_EntityProcessingSystem_h__
#define Artemis_EntityProcessingSystem_h__

#include "artemis/EntitySystem.h"
#include "artemis/EntitySystemType.h"

namespace artemis
{

/**
 * A typical entity system. Use this when you need to process entities possessing the
 * provided component types.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
class EntityProcessingSystem : public EntitySystem
{
public:
   EntityProcessingSystem(Aspect *aspect, EntitySystemType tp): EntitySystem(aspect, tp) {}

	/**
	 * Process a entity this system is interested in.
	 * @param e the entity to process.
	 */
protected:
   virtual void process(Entity *e) = 0;

	void processEntities(Bag<Entity *> *entities) override {
      size_t s = entities->size();
		for (size_t i = 0; i < s; ++i) {
			process(entities->get(i));
		}
	}
	
	bool checkProcessing() override {
		return true;
	}
	
};
}
#endif // Artemis_EntityProcessingSystem_h__
