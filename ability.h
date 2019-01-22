#ifndef _ABILITY_H_
#define _ABILITY_H_

#include <vector>
#include "handler.h"

class Player;
class Card;
class Ability{
 protected:
	Handler &h;
 	bool(*doAbilityWithTarget)(Handler &h, Player &target, char c) = nullptr;
 	bool(*doAbilityWithoutTarget)(Handler &h
 		, Player &player1, Player &player2) = nullptr;
 public:
 	Ability(Handler &h);
 	Ability(Handler &h, bool(*doAbilityWithTarget)(Handler &h
 			, Player &target, char c));
 	Ability(Handler &h, bool(*doAbilityWithoutTarget)(Handler &h
 			, Player &player1, Player &player2));

 	virtual bool doAbility(Player &target, char c);
 	virtual bool doAbility(Player &player1, Player &player2);
 	virtual ~Ability() = default;
};

#endif
