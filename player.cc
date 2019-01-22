#include "player.h"
#include "card.h"
#include "minion.h"
#include "ritual.h"
#include "enchantment.h"
#include "window.h"
#include <vector>
#include <iostream>


using namespace std;

Player::Player(string name){
	setName(name);
	/* initialize random seed: */
	srand (time(NULL));
}

int Player::getHP(){
	return hp;
}

int Player::getCurMagic(){
	return curMagic;
}

bool Player::isActive(){
	return active;
}

void Player::setActive(bool isActive){
	active = isActive;
}

void Player::setHP(int hp){
	this->hp = hp;
}

void Player::setCurMagic(int curMagic){
	this->curMagic = curMagic;
}

string Player::getName(){
	return name;
}

void Player::setName(string name){
	this->name = name;
}

vector<Card*> &Player::getDeck(){
	return deck;
}

vector<Card*> &Player::getHand(){
	return hand;
}

vector<Minion*> &Player::getBoardMinions(){
	return boardMinions;
}

vector<Minion*> &Player::getGrave(){
	return grave;
}

void Player::setRitual(Ritual *ritual){
	if (this->ritual != nullptr) delete this->ritual;
	this->ritual = ritual;
}

Ritual *Player::getRitual(){
	return ritual;
}

void Player::draw(){
	if (this->deck.size()==0){
		cout<<"No More Cards!!!!!!!!!"<<endl;
		return;
	}
	if (this->hand.size()<5){
		this->hand.push_back(this->deck[this->deck.size()-1]);
		this->deck.pop_back();
	}
	else{
		cout<<"To many card, no draw for you"<<endl;
	}

}

void Player::shuffle() {
	int lastPos =deck.size();
	for (int i=50; i>=0; --i) {
		int rand1 = rand() % lastPos;
		int rand2 = rand() % lastPos;
		swap(deck[rand1], deck[rand2]);
	}
}

void Player::draw_graphics(Xwindow &xw, const int x, const int y){
	stringstream ss;
	xw.fillRectangle(x, y, width, height);
	// mana box
	xw.fillRectangle(x + width - separator, y, 1, separator, Xwindow::Blue);
	xw.fillRectangle(x + width - separator, y + separator, separator, 1, Xwindow::Blue);
	ss << curMagic;
	xw.drawString(x + width - separator - 5 + separator/2, y + separator/2 + 3, ss.str());
	// hp box
	xw.fillRectangle(x + separator - 2, y, 1, separator, Xwindow::Blue);
	xw.fillRectangle(x, y + separator, separator, 1, Xwindow::Blue);
	ss.str(string());
	ss << hp;
	xw.drawString(x + separator/2 - 5, y + separator/2 + 3, ss.str());
	// player name
	string nameStr = name.substr(0, max_char);
	const int len = nameStr.size();
	const int res = (max_char - len)/2 * pixDensity;
	xw.drawString(x + res , y + separator*2, nameStr);
}

Player::~Player(){
	for (auto &c : deck) delete c;
	for (auto &c : boardMinions){
		Enchantment *e=dynamic_cast<Enchantment*>(c);
		if (e){
			Minion *toDelete= e;
			while (1){
				Minion *parent =toDelete->detach();
				delete toDelete;
				if (parent == nullptr){
					break;
				}
				toDelete = parent;
			}
		}else delete c;
	} 
	for (auto &c : hand) delete c;
	for (auto &c : grave) delete c;
	delete ritual;
}
