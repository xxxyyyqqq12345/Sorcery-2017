#ifndef ENCHANTMENT_H
#define ENCHANTMENT_H

#include <vector>
#include <string>
#include "card.h"
#include "minion.h"
#include "board.h"

class TriggeredAbility;
class Xwindow;
class Enchantment: public Minion {
	int actCost = 0;
	Minion *parentMinion = nullptr;
	void(*applyEffect)(Minion *pm, Enchantment *en) = nullptr;
	TriggeredAbility* trigAbility = nullptr;
	std::string enName;
	std::string atkRep;
	std::string defRep;
	bool silenced = false;

public:	
	Enchantment(void(*applyEffect)(Minion *pm, Enchantment *rm));
	Enchantment(TriggeredAbility* trigAbility);
	Enchantment(ActivatedAbility *actAbility);
	Enchantment(int atk, int def);

	int getATK() override;
	int getDEF() override;
	std::string &getAtkRep();
	std::string &getDefRep();
 	int getActCost() override;
 	int getAction() override;
 	std::string *getEnName();
 	std::vector<TriggeredAbility*> *getTrigAbilities() override;
 	ActivatedAbility *getActAbility() override;

	void setATK(int atk) override;
	void setDEF(int def) override;
	void setAtkRep(std::string atkRep);
	void setDefRep(std::string defRep);
 	void setActCost(int actCost) override;
	void setParent(Minion *m);
 	void setAction(int action) override;
 	void setEnName(std::string enName);
 	void setActAbility(ActivatedAbility *aa) override;
 	void addTrigAbility(TriggeredAbility *ta) override;

	Enchantment* attach(Minion *m);
 	Minion* detach();
	bool play(Handler &h, Player *target
			, char targetCard);
	Minion* find_basic_minion();
	card_template_t draw(Board *b) override;
	std::vector<card_template_t> draw_self(Board *b) override;
	void draw_graphics(Xwindow &xw, const int x, const int y) override;
	~Enchantment();
};

#endif
