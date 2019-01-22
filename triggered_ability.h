#ifndef _TRIGGERED_ABILITY_H_
#define _TRIGGERED_ABILITY_H_

#include "ability.h"
#include "trig_type.h"
class TriggeredAbility: public Ability{
	bool(*doAbilityPlayerMinion)(Handler &h
 		, Player &p, Minion &m);
	bool(*doAbilityMinionMinion)(Handler &h
 		, Minion &trigHolder, Minion *mTarget);
	TrigType condition;
 public:
 	using Ability::doAbility;
 	TriggeredAbility(Handler &h, bool(*doAbility)(Handler &h
 		, Player &p, Minion &m), TrigType condition);
 	TriggeredAbility(Handler &h, 
 		bool(*doAbility)(Handler &h, Player &active, Player &nonActive)
 		, TrigType condition);
 	TriggeredAbility(Handler &h, 
 		bool(*doAbilityMinionMinion)(Handler &h
 		, Minion &trigHolder, Minion *mTarget)
 		, TrigType condition);
 	bool doAbility(Player &p, Minion &m);
 	bool doAbility(Minion &trigHolder, Minion *mTarget);
 	TrigType getCondition();
 	void setCondition(TrigType condition);
};

#endif
