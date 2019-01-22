#include "ritual.h"
#include "handler.h"
#include "board.h"
#include "triggered_ability.h"
#include "window.h"

using namespace std;

void Ritual::reduceCharge(){
	charge -= actCost;
}

Ritual::Ritual(TriggeredAbility *triggeredAbility, int charge, int actCost):
	triggeredAbility{triggeredAbility}, charge{charge}, actCost{actCost} {}

void Ritual::draw_graphics(Xwindow &xw, const int x, const int y){
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
	xw.drawString(x + separator/2, y + separator * 2 - separator/2 + 3 * 2, "Ritual");
	// activate cost box
	int aaPadding = 30;
	int aaMaxChar = 5;
	xw.fillRectangle(x, y + separator * 3, separator, 1, Xwindow::Blue);
	xw.fillRectangle(x + separator, y + separator * 2, 1, separator, Xwindow::Blue);
	ss.str(std::string());
	ss << getActCost();
	xw.drawString(x - 5 + separator/2, y + separator*2 + separator/2 + 3, ss.str());
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
	// charge box
	xw.fillRectangle(width + x - separator, height + y - separator, separator, 1, Xwindow::Blue);
	xw.fillRectangle(width + x - separator, height + y - separator, 1, separator, Xwindow::Blue);
	ss.str(std::string());
	ss << getCharge();
	xw.drawString(x + width - separator + separator/2 - 4, y + height - separator + separator/2 + 3, 
		ss.str());
}

bool Ritual::play(Handler &h, Player *target, char targetCard){
	return h.card_played(*this);
}

int Ritual::getCharge(){
	return charge;
}

void Ritual::setCharge(int charge){
	this->charge = charge;
}

int Ritual::getActCost(){
	return actCost;
}

void Ritual::setActCost(int actCost){
	this->actCost = actCost;
}

bool Ritual::doAbility(Player &p, Minion &m){
	if (triggeredAbility->doAbility(p, m)){
		reduceCharge();
		return true;
	}
	return false;
}

bool Ritual::doAbility(Player &player1, Player &player2){
	if (triggeredAbility->doAbility(player1, player2)){
		reduceCharge();
		return true;
	}
	return false;
}

TrigType Ritual::getCondition(){
	return triggeredAbility->getCondition();
}

Ritual::~Ritual(){
	delete triggeredAbility;
}

card_template_t Ritual::draw(Board *b){
	return b->draw_ritual(this);
}
