#ifndef _SPELL_H_
#define _SPELL_H_

#include "card.h"
#include "board.h"

class ActivatedAbility;
class Spell: public Card{
	ActivatedAbility *activatedAbility;
 public:
 	Spell(ActivatedAbility *activatedAbility);
 	bool play(Handler &h, Player *target, char targetCard);
	card_template_t draw(Board *b);
 	bool doAbility(Player &target, char c);
 	bool doAbility(Player &player1, Player &player2);
	void draw_graphics(Xwindow &xw, const int x, const int y);
 	~Spell();
};

#endif
