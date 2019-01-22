#ifndef _BASIC_MINION_H_
#define _BASIC_MINION_H_

#include "minion.h"


class TriggeredAbility;
class Enchantment;
class ActivatedAbility;
class Xwindow;
class BasicMinion: public Minion{
	std::vector<TriggeredAbility*> trigAbilities;

 public:
 	BasicMinion(int atk, int def);
	 
 	std::vector<TriggeredAbility*> *getTrigAbilities() override;
 	ActivatedAbility *getActAbility() override;

 	void addTrigAbility(TriggeredAbility *ta) override;
 	void setActAbility(ActivatedAbility *aa) override;

 	Minion* detach() override;
	card_template_t draw(Board *b);
	std::vector<card_template_t> draw_self(Board *b) override;
	void draw_graphics(Xwindow &xw, const int x, const int y);
 	~BasicMinion();
};

#endif
