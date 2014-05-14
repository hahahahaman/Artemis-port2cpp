#ifndef Artemis_TeamManager_h__
#define Artemis_TeamManager_h__

#include "artemis/Manager.h"
#include "artemis/utils/Bag.h"
#include <map>

namespace artemis
{

/**
 * Use this class together with PlayerManager.
 * 
 * You may sometimes want to create teams in your game, so that
 * some players are team mates.
 * 
 * A player can only belong to a single team.
 * 
 * @author Arni Arent
 * @port   Vladimir Ivanov (ArCorvus)
 *
 */
template<typename T, typename P>
class TeamManager : public Manager
{
private:
   std::map<T, Bag<P> > playersByTeam;
	std::map<P, T> teamByPlayer;

public:
   TeamManager() : Manager(mtTeamManager) {}
	
protected:
   void initialize() override {}
	
public:
   T getTeam(P player)
   {
      auto it = teamByPlayer.find(player);
      if (it != teamByPlayer.end()) {
         return it->second;
      }
		return NULL;
	}
	
	void setTeam(P player, T team)
   {
		removeFromTeam(player);
		teamByPlayer[player] = team;
		Bag<P> &players = playersByTeam[team];
		players.add(player);
	}
	
	const Bag<T> * getPlayers(T team) const
   {
      auto it = playersByTeam.find(team);
      if (it != playersByTeam.end())
         return &it->second;
      return nullptr;
	}
	
	void removeFromTeam(P player)
   {
      auto it = teamByPlayer.find(player);
      if (it != teamByPlayer.end()) {
         auto itP = playersByTeam.find(it->second);
         if (itP != playersByTeam.end()) {
			   Bag<T> &players = itP->second;
				players.remove(player);
			}
         teamByPlayer.erase(it);
		}
	}

};
}
#endif // Artemis_TeamManager_h__
