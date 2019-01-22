#ifndef _RITUAL_H_
#define _RITUAL_H_

#include "card.h"
#include "board.h"
#include "trig_type.h"

class Minion;
class TriggeredAbility;
class Ritual: public Card{
	TriggeredAbility *triggeredAbility;
	int charge;
	int actCost;
	void reduceCharge();
 public:
 	Ritual(TriggeredAbility *triggeredAbility, int charge, int actCost);
 	bool play(Handler &h, Player *target, char targetCard);
	card_template_t draw(Board *b);
 	bool doAbility(Player &p, Minion &m);
 	bool doAbility(Player &player1, Player &player2);
 	TrigType getCondition();
 	void setActCost(int actCost);
 	void setCharge(int charge);
 	int getCharge();
 	int getActCost();
	void draw_graphics(Xwindow &xw, const int x, const int y);
 	~Ritual();
};


#endif
