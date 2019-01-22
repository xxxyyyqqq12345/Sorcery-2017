#include "basic_minion.h"
#include "minion.h"
#include "activated_ability.h"
#include "triggered_ability.h"
#include "window.h"

using namespace std;

BasicMinion::BasicMinion(int atk, int def): Minion{atk,def} {}

vector<TriggeredAbility*> *BasicMinion::getTrigAbilities(){
	return &trigAbilities;
}

ActivatedAbility *BasicMinion::getActAbility(){
	return actAbility;
}

void BasicMinion::addTrigAbility(TriggeredAbility *ta){
	trigAbilities.push_back(ta);
}
void BasicMinion::setActAbility(ActivatedAbility *aa){
	if (actAbility != nullptr) delete actAbility;
	actAbility = aa;
}

Minion* BasicMinion::detach() {
	return nullptr;
}

BasicMinion::~BasicMinion(){
	for (auto &ta: trigAbilities) delete ta;
	delete actAbility;
}

card_template_t BasicMinion::draw(Board *b){
	if (this->actAbility!=nullptr){
		return b->draw_minion_ae(this);

	}
	else if(this->trigAbilities.size()>0) {
		return b->draw_minion_te(this);
	}
	else{
		return b->draw_minion_ne(this);
	}
}
vector<card_template_t> BasicMinion::draw_self(Board *b){
	vector<card_template_t> tmp;
	return tmp;
}

void BasicMinion::draw_graphics(Xwindow &xw, const int x, const int y){
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
	xw.drawString(x + separator/2, y + separator * 2 - separator/2 + 3 * 2, "Minion");
	// activated ability cost box
	int aaPadding = 0;
	int aaMaxChar = 0;
	if (actAbility){
		xw.fillRectangle(x, y + separator * 3, separator, 1, Xwindow::Blue);
		xw.fillRectangle(x + separator, y + separator * 2, 1, separator, Xwindow::Blue);
		ss.str(string());
		ss << getActCost();
		xw.drawString(x - 5 + separator/2, y + separator*2 + separator/2 + 3, ss.str());
		aaPadding = 30;
		aaMaxChar = 5;
	}

	// description
	string des = getDescription();
	unsigned int count = 0;
	int deltaY = 0;
	while (count < des.size()){
		xw.drawString(x + separator/2 - 3 + aaPadding
			, y + separator * 3 - separator/2 + 3 * 2 + deltaY
			, des.substr(count, max_char - aaMaxChar));
		deltaY += 13;
		count += max_char - aaMaxChar;
	}
	// atk box
	xw.fillRectangle(x, height + y - separator, separator, 1, Xwindow::Blue);
	xw.fillRectangle(x + separator, height + y - separator, 1, separator, Xwindow::Blue);
	ss.str(string());
	ss << getATK();
	xw.drawString(x + separator/2 - 4, y + height - separator + separator/2 + 3, ss.str());
	// def box
	xw.fillRectangle(width + x - separator, height + y - separator, separator, 1, Xwindow::Blue);
	xw.fillRectangle(width + x - separator, height + y - separator, 1, separator, Xwindow::Blue);
	ss.str(string());
	ss << getDEF();
	xw.drawString(x + width - separator + separator/2 - 4, y + height - separator + separator/2 + 3, 
		ss.str());
}