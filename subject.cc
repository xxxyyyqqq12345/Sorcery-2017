#include "subject.h"
#include "observer.h"
#include "board_place.h"
#include "player.h"

void Subject::attach(Observer *o) {
	observers.push_back(o);
}

void Subject::notifyObservers(BoardPlace bp, Player *p){
	for (auto &ob: observers){
		ob->notify(bp, p);
	}
}

void Subject::notifyObservers(Player *AP, Player *NAP){
	for (auto &ob: observers){
		ob->notify(AP, NAP);
	}
}