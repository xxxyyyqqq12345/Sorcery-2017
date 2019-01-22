#include <iostream>
#include "spell.h"
#include "handler.h"
#include "activated_ability.h"
#include "window.h"

using namespace std;

Spell::Spell(ActivatedAbility *activatedAbility):
	activatedAbility{activatedAbility} {}

void Spell::draw_graphics(Xwindow &xw, const int x, const int y){
	stringstream ss;
	xw.fillRectangle(x, y, width, height);
	//cost barrier
	xw.fillRectangle(x + width - separator - 5, y, 1, separator, Xwindow::Blue);
	ss << getCP();
	xw.drawString(x + width - separator - 5 + separator/2, y + separator/2 + 3, ss.str());
	// card name
	xw.fillRectangle(x, y + separator, width, 1, Xwindow::Blue);
	xw.drawString(x + separator/2, y + separator - separator/2 + 3, getName());
	// card type
	xw.fillRectangle(x, y + separator * 2, width, 1, Xwindow::Blue);
	xw.drawString(x + separator/2, y + separator * 2 - separator/2 + 3 * 2, "Spell");
	// description
	string des = getDescription();
	unsigned int count = 0;
	int deltaY = 0;
	while (count < des.size()){
		xw.drawString(x + separator/2 - 3
			, y + separator * 3 - separator/2 + 3 * 2 + deltaY
			, des.substr(count, max_char));
		deltaY += 13;
		count += max_char;
	}
}

bool Spell::play(Handler &h, Player *target, char targetCard){
	return h.card_played(*this, target, targetCard);
}

bool Spell::doAbility(Player &target, char c){
	return activatedAbility->doAbility(target, c);
}

bool Spell::doAbility(Player &player1, Player &player2){
	return activatedAbility->doAbility(player1, player2);
}

card_template_t Spell::draw(Board *b){
	return b->draw_spell(this);
}

Spell::~Spell(){
	delete activatedAbility;
}
