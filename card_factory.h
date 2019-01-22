#ifndef LOADCARD_H
#define LOADCARD_H
#include <vector>
#include <string>
#include "handler.h"
#include "player.h"
#include "card.h"
#include "basic_minion.h"

class CardFactory{
		Handler &h;
	public:
		CardFactory(Handler &h);
		/*
			loadCard(p, cardName) returns a Card object pointer that
			defines the card of name cardName under the ownership of player p
		Effects:	returns an object pointer that points to heap allocated data.
					User should free.
		*/
		Card* loadCard(Player &p, const std::string &cardName);
		/* 
			loadDeck(p, deckFile) loads the deck of player p using the file
			named deckFile.
		Effects:	the Card pointers loaded into p's deck consists of pointers
					that point to data on the heap. User should free.
		*/
		void loadDeck(Player &p, const std::string &deckFile);
};


#endif
