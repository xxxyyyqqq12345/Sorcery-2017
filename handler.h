#ifndef _HANDLER_H_
#define _HANDLER_H_
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "spell.h"
#include "player.h"
#include "minion.h"
#include "ritual.h"
#include "trig_type.h"
#include "enchantment.h"
#include "board.h"
#include "subject.h"

class Spell;
class Handler: public Subject {
	std::vector<Card*> cardDump;
	Player *p1;
	Player *p2;
	void checkMinions(Minion &m, TrigType tt);
	void checkMinions(Player &player, TrigType tt);
	void checkRitualsFromMinionTrig(Player &p, Minion &m, TrigType tt);
 public:
 	Handler(Player *P1, Player *P2);
	void start_turn(Player &active, Player &nonActive);
	void end_turn(Player &active, Player &nonActive);
	Minion *moveToGrave(Player &p, int pCard);
	Minion *moveToGrave(Minion &m);
	void minion_enters_play(Player &p, Minion &m);
	Minion *minion_leaves_play(Player &p, Minion &m);
	void minion_attack(Minion *attackingMinion, Player &target, int targetCard = 0);
	bool card_played(Minion &M, Player *target, char targetCard);
	bool card_played(Spell &S, Player *target, char targetCard);
	bool card_played(Enchantment &E, Player *target, char targetCard);
	bool card_played(Ritual &R);
	void dumpCard(Card *c);
	~Handler();
};
#endif
