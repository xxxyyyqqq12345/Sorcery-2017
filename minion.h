#ifndef _MINION_H_
#define _MINION_H_

#include "card.h"
#include <vector>
#include "board.h"

class TriggeredAbility;
class ActivatedAbility;

//class Enchantment;
class Minion: public Card{
 protected:
	int atk = 0;
	int def = 0;
	int defaultAtk = 0;
	int defaultDef = 0;
	int action = 0;
	int actCost = -1;
	bool onBoard = false; // keeps track of whether the minion is on the board
	ActivatedAbility *actAbility = nullptr;

 public:
 	Minion() = default;
	Minion(ActivatedAbility *aa);
 	Minion(int atk, int def);

 	virtual int getActCost();
 	virtual int getATK();
 	virtual int getDEF();
 	virtual int getAction();
 	virtual std::vector<TriggeredAbility*> *getTrigAbilities() = 0;
 	virtual ActivatedAbility *getActAbility() = 0;
 	bool isOnBoard();

 	virtual void setActCost(int actCost);
 	virtual void setATK(int atk);
 	virtual void setDEF(int def);
 	virtual void setAction(int action);
 	virtual void setActAbility(ActivatedAbility *aa) = 0;
 	virtual void addTrigAbility(TriggeredAbility *ta) = 0;
 	void setOnBoard(bool onBoard);

	/*
		detach() returns the parent minion. This method is used to get the 
		parent minion of an enchantment (which can also be an enchanement).
		Repeated calls will eventually return null when the concrete 
		BasicMinion is reach
	*/
 	virtual Minion* detach() = 0;
	virtual std::vector<card_template_t> draw_self(Board *b)=0;
 	virtual bool play(Handler &h, Player *target, char targetCard);
 	virtual ~Minion() = default;
};

#endif
