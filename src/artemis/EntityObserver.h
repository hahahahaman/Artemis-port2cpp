#ifndef Artemis_EntityObserver_h__
#define Artemis_EntityObserver_h__

namespace artemis
{
class Entity;
/**
 * Entity Observer.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
class EntityObserver
{
public:
   virtual ~EntityObserver() {}
	virtual void added(Entity *e) = 0;
	virtual void changed(Entity *e) = 0;
	virtual void deleted(Entity *e) = 0;
	virtual void enabled(Entity *e) = 0;
	virtual void disabled(Entity *e) = 0;
};
}
#endif // Artemis_EntityObserver_h__
