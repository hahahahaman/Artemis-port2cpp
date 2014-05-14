#ifndef Artemis_Manager_h__
#define Artemis_Manager_h__

#include "artemis/EntityObserver.h"
#include "artemis/ManagerType.h"

namespace artemis
{
class World;
/**
 * Manager.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 * 
 */
class Manager : public EntityObserver
{
   friend class World;
private:
   const ManagerType mType; // TODO: move to static or template param
protected:
   World *world;
	virtual void initialize() = 0;
	void setWorld(World *world) { this->world = world; }
	World * getWorld() { return world; }
	
public:
   Manager(ManagerType tp): mType(tp) {}
   ~Manager() {}
   ManagerType getType() const { return mType; }
   void added(Entity *e) override {}
   void changed(Entity *e) override {}
	void deleted(Entity *e) override {}
	void disabled(Entity *e) override {}
	void enabled(Entity *e) override {}
};
}
#endif // Artemis_Manager_h__