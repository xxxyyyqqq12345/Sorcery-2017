#include "activated_ability.h"
#include "ability.h"
#include "minion.h"

ActivatedAbility::ActivatedAbility(Handler &h
	, bool(*doAbility)(Handler &h, Player &target, char c)): 
	Ability{h, doAbility} {}
	
ActivatedAbility::ActivatedAbility(Handler &h
	, bool(*doAbility)(Handler &h, Player &player1, Player &player2)): 
	Ability{h, doAbility} {}

ActivatedAbility::ActivatedAbility(Handler &h, bool(*doAbilityOnMinionSelf)(Handler &h
 		, Minion *m)):
	Ability{h}, doAbilityOnMinionSelf{doAbilityOnMinionSelf} {}


bool ActivatedAbility::doAbility(Player &target, char c, Minion *m){
	if (doAbilityOnMinionSelf) return doAbilityOnMinionSelf(h, m);
	else return doAbility(target, c);
}

bool ActivatedAbility::doAbility(Player &p1, Player &p2, Minion *m){
	if (doAbilityOnMinionSelf) return doAbilityOnMinionSelf(h, m);
	else return doAbility(p1, p2);
}
