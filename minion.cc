#include <iostream>
#include "minion.h"
#include "handler.h"
#include "board.h"

using namespace std;

Minion::Minion(int atk, int def): atk{atk}, def{def} 
	, defaultAtk{atk}, defaultDef{def} {}

Minion::Minion(ActivatedAbility *aa): actAbility{aa} {}

int Minion::getActCost(){
	return actCost;
}

int Minion::getATK(){ 
	return atk; 
}

int Minion::getDEF(){
	return def;
}

int Minion::getAction(){
	return action;
}

void Minion::setActCost(int actCost){
	this->actCost = actCost;
}

void Minion::setATK(int atk){
	this->atk = atk;
}

void Minion::setDEF(int def){
	this->def = def;
}

void Minion::setAction(int action){
	this->action = action;
}

bool Minion::play(Handler &h, Player *target
				, char targetCard){
	if (target || targetCard != '0'){
		cout <<"Minions cannot have targets when played"<<endl;
		return false;
	}
	return h.card_played(*this, target, targetCard);
}

void Minion::setOnBoard(bool onBoard){
	this->onBoard = onBoard;
}

bool Minion::isOnBoard(){
	return onBoard;
}
