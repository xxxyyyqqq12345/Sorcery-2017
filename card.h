#ifndef _CARD_H_
#define _CARD_H_

#include <sstream>
#include <string>
#include "ascii_graphics.h"

class Board;
class Player;
class Handler;
class Xwindow;
class Card{
	Player *owner;
	int cp;
	std::string name;
	std::string description;
 public:
 	// constants for graphical display
 	static const int separator = 30, width = 200, height = 144, max_char=30;

	virtual Player &getOwner();
	virtual int getCP();
	virtual std::string &getDescription();
	virtual std::string &getName();

	virtual void setOwner(Player *p);
	virtual void setCP(int cp);
	virtual void setDescription(std::string des);
	virtual void setName(std::string name);

	/*	
		play(h, target, targetCard) called when this card is played from the hand.
		target and targetCard are optional paramters for cards that specify a target 
	*/
	virtual bool play(Handler &h, Player *target
					, char targetCard) = 0;
	/*
		draw(b) draws this the card on the text display using b
	*/
	virtual card_template_t draw(Board *b)=0;
	/*
		draw_graphics (xw, x, y) draws this card on the graphical display
		on coordinates (x,y) using xw.
	*/
	virtual void draw_graphics(Xwindow &xw, const int x, const int y) = 0;
	virtual ~Card() = default;
};

#endif
