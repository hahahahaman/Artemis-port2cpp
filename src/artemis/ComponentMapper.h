#ifndef Artemis_ComponentMapper_h__
#define Artemis_ComponentMapper_h__

#include "artemis/ComponentType.h"
#include "artemis/utils/Bag.h"
#include "artemis/Component.h"
#include "artemis/Entity.h"

namespace artemis
{
class World;
//class Entity;
/**
 * High performance component retrieval from entities. Use this wherever you
 * need to retrieve components from entities often and fast.
 *
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 * @param <A> the class type of the component
 */
class ComponentMapperHelper
{
public:
   static Bag<Component *> *getComponents(World *w, ComponentType tp);
};

class BaseComponentMapper
{
   friend class World;
protected:
   virtual void init(World *) = 0;
};

template<typename T, ComponentType cType>
class ComponentMapper : public BaseComponentMapper
{
private:
   Bag<Component *> *components;

protected:
   void init(World *w) override
   {
      components = ComponentMapperHelper::getComponents(w, cType);
   }
public:
   ComponentMapper(): components(nullptr) {}
	/**
	 * Fast but unsafe retrieval of a component for this entity.
	 * No bounding checks, so this could throw an ArrayIndexOutOfBoundsExeption,
	 * however in most scenarios you already know the entity possesses this component.
	 *
	 * @param e the entity that should possess the component
	 * @return the instance of the component
	 */
   T * get(Entity *e) {
		return static_cast<T *>(components->get(e->getId()));
	}

	/**
	 * Fast and safe retrieval of a component for this entity.
	 * If the entity does not have this component then null is returned.
	 *
	 * @param e the entity that should possess the component
	 * @return the instance of the component
	 */
	T * getSafe(Entity *e) {
		if(components->isIndexWithinBounds(e->getId())) {
			return static_cast<T *>(components->get(e->getId()));
		}
		return nullptr;
	}

	/**
	 * Checks if the entity has this type of component.
	 * @param e the entity to check
	 * @return true if the entity has this component type, false if it doesn't.
	 */
	bool has(Entity *e) {
		return getSafe(e) != nullptr;
	}

	/**
	 * Returns a component mapper for this type of components.
	 *
	 * @param type the type of components this mapper uses.
	 * @param world the world that this component mapper should use.
	 * @return a new mapper.
	 * /
   static ComponentMapper<T, cType> * getFor(World *world) {
		return new ComponentMapper<T, cType>(world);
	}//*/

};
}
#endif // Artemis_ComponentMapper_h__
