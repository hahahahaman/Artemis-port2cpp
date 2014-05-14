#ifndef Artemis_TagManager_h__
#define Artemis_TagManager_h__

#include "artemis/Manager.h"
#include <map>

namespace artemis
{

/**
 * If you need to tag any entity, use this. A typical usage would be to tag
 * entities such as "PLAYER", "BOSS" or something that is very unique.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
template<typename T>
class TagManager : public Manager
{
private:
   std::map<T, Entity *> entitiesByTag;
	std::map<Entity *, T> tagsByEntity;

public:
   TagManager() : Manager(mtTagManager) {};

	void registerTag(T tag, Entity *e)
   {
		entitiesByTag[tag] = e;
		tagsByEntity[e] = tag;
	}

	void unregister(T tag)
   {
      auto it = entitiesByTag.find(tag);
      if (it != entitiesByTag.end()) {
         tagsByEntity.erase(it->second);
         entitiesByTag.erase(it);
      }
	}

	bool isRegistered(T tag)
   {
		return (entitiesByTag.find(tag) != entitiesByTag.end());
	}

	Entity * getEntity(T tag)
   {
      auto it = entitiesByTag.find(tag);
      if (it != entitiesByTag.end())
         return it->second;
		return nullptr;
	}
	
	const std::map<T, Entity *> * getRegisteredTags() const
   {
		return &entitiesByTag;
	}
	
	void deleted(Entity *e) override
   {
      auto it = tagsByEntity.find(e);
      if (it != tagsByEntity.end()) {
			entitiesByTag.erase(it->second);
		}
	}

protected:
   void initialize() override {}

};
}
#endif // Artemis_TagManager_h__
