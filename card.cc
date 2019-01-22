#include "card.h"
#include "player.h"

using namespace std;

Player &Card::getOwner(){
	return *owner;
}

int Card::getCP(){
	return cp;
}

string &Card::getDescription() {
	return description;
}

string &Card::getName(){
	return name;
}

void Card::setOwner(Player *p){
	owner = p;
}

void Card::setCP(int cp){
	this->cp = cp;
}

void Card::setDescription(string des){
	description = des;
}

void Card::setName(string name){
	this->name = name;
}
