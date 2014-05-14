#ifndef Artemis_PlayerManager_h__
#define Artemis_PlayerManager_h__

#include "artemis/Manager.h"
#include "artemis/utils/Bag.h"
#include <map>

namespace artemis
{

/**
 * You may sometimes want to specify to which player an entity belongs to.
 * 
 * An entity can only belong to a single player at a time.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
template<typename P>
class PlayerManager : public Manager
{
	std::map<Entity *, P> playerByEntity;
	std::map<P, Bag<Entity *> > entitiesByPlayer;

public:
   PlayerManager(): Manager(mtPlayerManager) {}
	void setPlayer(Entity *e, P player);
	Bag<Entity *> * getEntitiesOfPlayer(P player);
	void removeFromPlayer(Entity *e);
	P getPlayer(Entity *e);
   void deleted(Entity *e) override;

protected:
   void initialize() override {}

};

template<typename P>
void PlayerManager<P>::setPlayer(Entity *e, P player)
{
   playerByEntity[e] = player;
   Bag<Entity *> &entities = entitiesByPlayer[player];
   entities.add(e);
}

template<typename P>
Bag<Entity *> * PlayerManager<P>::getEntitiesOfPlayer(P player)
{
   auto it = entitiesByPlayer.find(player);
   if (it != entitiesByPlayer.end()) {
      return &it->second;
   }
   return nullptr;
}

template<typename P>
void PlayerManager<P>::removeFromPlayer(Entity *e)
{
   auto it = playerByEntity.find(e);
   if (it != playerByEntity.end()) {
      auto itE = entitiesByPlayer.find(it->second);
      if (itE != entitiesByPlayer.end()) {
         Bag<Entity *> &entities = itE->second;
         entities.remove(e);
      }
   }
}

template<typename P>
P PlayerManager<P>::getPlayer(Entity *e)
{
   auto it = playerByEntity.find(e);
   if (it != playerByEntity.end())
      return it->second;
   return NULL; // ???
}

template<typename P>
void PlayerManager<P>::deleted(Entity *e)
{
   Manager::deleted(e);
   removeFromPlayer(e);
}

}
#endif // Artemis_PlayerManager_h__
