#include "triggered_ability.h"
#include "ability.h"
#include "trig_type.h"

TriggeredAbility::TriggeredAbility(Handler &h
	, bool(*doAbilityPlayerMinion)(Handler &h, Player &p, Minion &m)
	, TrigType condition): 
	Ability{h}, doAbilityPlayerMinion{doAbilityPlayerMinion}, condition{condition} {}
	
TriggeredAbility::TriggeredAbility(Handler &h
	, bool(*doAbility)(Handler &h, Player &active, Player &nonActive)
	, TrigType condition): 
	Ability{h, doAbility}, condition{condition}  {}

TriggeredAbility::TriggeredAbility(Handler &h, 
	bool(*doAbilityMinionMinion)(Handler &h
	, Minion &trigHolder, Minion *mTarget)
	, TrigType condition):
	Ability{h}, doAbilityMinionMinion{doAbilityMinionMinion}, condition{condition} {}

TrigType TriggeredAbility::getCondition(){
	return condition;
}

bool TriggeredAbility::doAbility(Player &p, Minion &m){
	return doAbilityPlayerMinion(h, p, m);
}

bool TriggeredAbility::doAbility(Minion &trigHolder, Minion *mTarget){
	return doAbilityMinionMinion(h, trigHolder, mTarget);
}

void TriggeredAbility::setCondition(TrigType condition){
	this->condition = condition;
}
