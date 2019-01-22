#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
#include <string>
#include <cstdlib>
#include <utility>
#include "minion.h"


class Card;
class Ritual;
class Player{
	int hp = 20;
	int curMagic = 3;
	bool active = false;
	Ritual *ritual = nullptr;
	std::string name;
	std::vector<Card*> deck;
	std::vector<Card*> hand;
	std::vector<Minion*> boardMinions;
	std::vector<Minion*> grave;
 	const int separator = 30, width = 200, height = 144
	 		, max_char=30;
	const float pixDensity = 7;
	//Ritual boardRitual;
 public:
 	Player(std::string name);
 	int getHP();
 	int getCurMagic();
 	bool isActive();
 	void setHP(int hp);
 	void setCurMagic(int curMagic);
 	void setActive(bool isActive);
 	std::string getName();
 	void setName(std::string name);
 	std::vector<Card*> &getDeck();
 	std::vector<Card*> &getHand();
 	std::vector<Minion*> &getBoardMinions();
 	std::vector<Minion*> &getGrave();
	void draw();
	void draw_graphics(Xwindow &xw, const int x, const int y);
	void setRitual(Ritual *ritual);
 	Ritual *getRitual();
 	void shuffle();
 	~Player();
};

#endif
