#ifndef _ACTIVATED_ABILITY_H_
#define _ACTIVATED_ABILITY_H_

#include "ability.h"

class Player;
class Minion;
class ActivatedAbility: public Ability{
 	bool(*doAbilityOnMinionSelf)(Handler &h
 		, Minion *m) = nullptr;
 public:
 	using Ability::doAbility;
 	ActivatedAbility(Handler &h, bool(*doAbility)(Handler &h
 		, Player &target, char c));
 	ActivatedAbility(Handler &h
 		, bool(*doAbility)(Handler &h, Player &player1, Player &player2));
 	ActivatedAbility(Handler &h, bool(*doAbilityOnMinionSelf)(Handler &h
 		, Minion *m));

 	bool doAbility(Player &target, char c, Minion *m);
 	bool doAbility(Player &player1, Player &player2, Minion *m);
};
#endif
