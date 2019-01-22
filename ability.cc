#include <iostream>
#include "ability.h"
#include "player.h"
#include "handler.h"

using namespace std;

Ability::Ability(Handler &h): h{h} {}

Ability::Ability(Handler &h, bool(*doAbilityWithTarget)(Handler &h
	, Player &target, char c)): 
	h{h}, doAbilityWithTarget{doAbilityWithTarget} {}

Ability::Ability(Handler &h, bool(*doAbilityWithoutTarget)(Handler &h 
	,Player &player1, Player &player2)):
	h{h}, doAbilityWithoutTarget{doAbilityWithoutTarget} {}

bool Ability::doAbility(Player &target, char c){
	if (doAbilityWithTarget) return doAbilityWithTarget(h, target, c);
	else{
		cout << "Invalid command"<<endl;
		return false;
	}
}

bool Ability::doAbility(Player &player1, Player &player2){
	if (doAbilityWithoutTarget) return doAbilityWithoutTarget(h, player1, player2);
	else{
		cout << "Invalid command"<<endl;
		return false;
	}
}
