#ifndef Artemis_GroupManager_h__
#define Artemis_GroupManager_h__

#include "artemis/Manager.h"
#include "artemis/utils/Bag.h"
#include <map>
#include <vector>

namespace artemis
{
/**
 * If you need to group your entities together, e.g. tanks going into "units" group or explosions into "effects",
 * then use this manager. You must retrieve it using world instance.
 * 
 * A entity can be assigned to more than one group.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
template<typename G>
class GroupManager : public Manager
{
private:
   std::map<G, Bag<Entity *> > entitiesByGroup;
	std::map<Entity *, std::vector<G> > groupsByEntity;

public:
   GroupManager(): Manager(mtGroupManager) {}

protected:
   void initialize() override {}
	
public:	
	/**
	 * Set the group of the entity.
	 * 
	 * @param group group to add the entity into.
	 * @param e entity to add into the group.
	 */
   void add(Entity *e, G group)
   {
      Bag<Entity *> &entities = entitiesByGroup[group];
      entities.add(e);

      std::vector<G> &groups = groupsByEntity[e];
      groups.push_back(group);
   }
	
	/**
	 * Remove the entity from the specified group.
	 * @param e
	 * @param group
	 */
	void remove(Entity *e, G group)
   {
      auto itE = entitiesByGroup.find(group);
      if (itE != entitiesByGroup.end()) {
         Bag<Entity *> &entities = itE->second;
         entities.remove(e);
      }

      auto itG = groupsByEntity.find(e);
      if (itG != groupsByEntity.end()) {
         std::vector<G> &groups = itG->second;
         groups.erase(groups.find(group));
      }
   }
	
	void removeFromAllGroups(Entity *e)
   {
      auto itG = groupsByEntity.find(e);
      if (itG != groupsByEntity.end()) {
         std::vector<G> &groups = itG->second;
         for (size_t i = 0; groups.size() > i; i++) {
            auto itE = entitiesByGroup.find(groups[i]);
            if (itE != entitiesByGroup.end()) {
               Bag<Entity *> &entities = itE->second;
               entities.remove(e);
            }
         }
         groups.clear();
      }
   }
	
	/**
	 * Get all entities that belong to the provided group.
	 * @param group name of the group.
	 * @return read-only bag of entities belonging to the group.
	 */
	const Bag<Entity *> * getEntities(G group)
   {
      auto itE = entitiesByGroup.find(group);
      if (itE != entitiesByGroup.end()) {
         return &itE->second;
      }
      return nullptr;
   }
	
	/**
	 * @param e entity
	 * @return the groups the entity belongs to, null if none.
	 */
	const Bag<G> * getGroups(Entity *e)
   {
      auto itG = groupsByEntity.find(e);
      if (itG != groupsByEntity.end()) {
         return &itG->second;
      }
      return nullptr;
   }
	
	/**
	 * Checks if the entity belongs to any group.
	 * @param e the entity to check.
	 * @return true if it is in any group, false if none.
	 */
	bool isInAnyGroup(Entity *e)
   {
      return (getGroups(e) != nullptr);
   }
	
	/**
	 * Check if the entity is in the supplied group.
	 * @param group the group to check in.
	 * @param e the entity to check for.
	 * @return true if the entity is in the supplied group, false if not.
	 */
	bool isInGroup(Entity *e, G group)
   {
      auto itG = groupsByEntity.find(e);
      if (itG != groupsByEntity.end()) {
         std::vector<G> &groups = itG->second;
         for(size_t i = 0; groups.size() > i; i++) {
            if (group == groups.get(i))
               return true;
         }
      }
      return false;
   }

   void deleted(Entity *e) override
   {
      Manager::deleted(e);
      removeFromAllGroups(e);
   }
};
}
#endif // Artemis_GroupManager_h__
