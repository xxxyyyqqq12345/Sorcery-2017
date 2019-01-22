#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <exception>


#include "handler.h"
#include "card.h"
#include "board.h"
#include "minion.h"
#include "basic_minion.h"
#include "player.h"
#include "card_factory.h"
#include "spell.h"
#include "ability.h"
#include "activated_ability.h"
#include "ritual.h"
#include "triggered_ability.h"
#include "window.h"
#include "board_place.h"

using namespace std;

int main(int args, char *commands[]){
	bool testing=false;
	bool quit=false;
	Xwindow *xw = nullptr;
	
	string command = "";
	string player1Name = "";
	string player2Name = "";
	istringstream initFromFile {command};

	for (int i=1; i<args; ++i) {
		string comArg = commands[i];
		if (comArg.compare("-init") == 0) {
			++i;
			if (i != args) {
				string filename = commands[i];
				ifstream filecontent{filename};
				getline(filecontent, player1Name);
				getline(filecontent, player2Name);
				string order = "";
				while (getline(filecontent, command)) {
					order = order + command + '\n';
				}
			initFromFile.str (order);
			}
		}
	}


	if (player1Name == "") {
		cout << "Please Enter the Name of Player 1: ";
		getline(cin, player1Name);
	}
	if (player2Name == "") {
		cout << "Please Enter the Name of Player 2: ";
		getline(cin, player2Name);
	}

	Player player1{player1Name};
	player1.setActive(true);
	Player player2{player2Name};
	player2.setActive(false);

	Board board;
	Handler handler{&player1, &player2};
	handler.attach(&board);
	Player *curPlayer = &player1;
	Player *nonActivePlayer = &player2;

	istringstream iss;
	bool turn_end=0;


	CardFactory cardFactory{handler};

	for (int i=1; i<args; ++i) {
		string comArg = commands[i];
		if (comArg.compare("-deck1") == 0) {
			++i;
			if (i!=args) {
				string deckfile = commands[i];
				cardFactory.loadDeck(player1, deckfile);
			}
		}
		else if (comArg.compare("-deck2") == 0) {
			++i;
			if (i!=args) {
				string deckfile = commands[i];
				cardFactory.loadDeck(player2, deckfile);
			}
		}
		else if (comArg.compare("-testing") == 0) {
			testing = true;
		}
		else if (comArg.compare("-graphics") == 0) {
			xw= new Xwindow{1200, 850};

			// draw river
			xw->fillRectangle(0,308,1200,80,Xwindow::Blue);
			xw->drawString(575,346,"Sorcery",Xwindow::White);
			board.attach_graphics(xw);
		}
	}

	// if the player have not load a deck yet, load the default deck.
	if (player1.getDeck().size() == 0) {
		cardFactory.loadDeck(player1, "default.deck");
	}
	if (player2.getDeck().size() == 0) {
		cardFactory.loadDeck(player2, "default.deck");
	}

	// shuffle deck if not in testing mode
	if (!testing) {
		player1.shuffle();
		player2.shuffle();
	}
	

	for (int i=0;i<=4;++i){
		player1.draw();
		player2.draw();
	}


	while(!quit){
		curPlayer->setCurMagic(curPlayer->getCurMagic()+1);
		
		curPlayer->draw();
		
		cout<<"Turn start:"<< curPlayer->getName()<<endl;
		handler.start_turn(*curPlayer, *nonActivePlayer);

		board.display_board(nonActivePlayer,curPlayer);
		board.display_hand(curPlayer);
		turn_end=0;
		while (turn_end!=1){
			//when health goes to 0 break all;
			if (player1.getHP() <= 0) {
				cout << player2Name << " has won!" << endl;
				cout << "Thank you for playing!" << endl;
				quit=true;
				break;
			}
			if (player2.getHP() <= 0) {
				cout << player1Name << " has won!" << endl;
				cout << "Thank you for playing!" << endl;
				quit=true;
				break;
			}

			if (getline(initFromFile,command)) {
				iss=istringstream(command);
				iss>>command;
			}
			else {
				getline(cin,command); 
				iss=istringstream(command);
				iss>>command;
			}
			if (command=="quit"){
				quit=true;
				break;
			}
			else if (command=="help"){
				ifstream hs{"help.txt"};
				string help;
				string temp;
				while (getline(hs, temp)) {
					help = help + "\n" + temp;
				}
				cout << help << endl;
				board.notify(curPlayer,nonActivePlayer);
			}
			else if (command=="end"){
				turn_end=1;
				cout<<"Debug: Turn end:"<< curPlayer->getName()<<endl;
			}

			else if (command=="play"){
				Card *card;
				int var1 = 0;
				int var2 = 0;
				char arg3 = '0';
				iss>>var1;
				iss>>var2;
				iss>>arg3;
				if (var1 >= 1 && static_cast<unsigned>(var1)<=curPlayer->getHand().size()){
					card = (curPlayer->getHand())[var1-1];
					bool validPlayer = true;
					Player *player = nullptr;
					if (var2 == 1) player = &player1;
					else if (var2 == 2) player = &player2;
					else if (var2 != 0) {
						cout <<"Not a valid player"<<endl;
						validPlayer = false;
					}
					if (validPlayer){
						if (!player ||
							(static_cast<unsigned>(arg3 - '1') < player->getBoardMinions().size()
							|| arg3 == 'r')){
							if (card->getCP()<=curPlayer->getCurMagic()){
								if (card->play(handler, player, arg3)){
									curPlayer->setCurMagic(curPlayer->getCurMagic()-card->getCP());
									curPlayer->getHand().erase(curPlayer->getHand().begin() + var1-1);
									handler.notifyObservers(BoardPlace::Hand, curPlayer);
									handler.notifyObservers(BoardPlace::Player, curPlayer);
								}
							}
							else{
								if (testing){
									if (dynamic_cast<Spell*>(card) && card->play(handler, player, arg3)){
										curPlayer->setCurMagic(0);
										curPlayer->getHand().erase(curPlayer->getHand().begin() + var1-1);
										handler.notifyObservers(BoardPlace::Hand, curPlayer);
										handler.notifyObservers(BoardPlace::Player, curPlayer);
									}
									else {
										cout << "Not enough Mana to play" << endl;
									}
								}
								else{
									cout<<"Not enough Mana to play"<<endl;
								}
							}
						}else cout << "No such target"<<endl;
					}
				}
				else{
					cout<<"No such Card"<<endl;
				}
			}
			else if (command=="use"){
				int var1 = 0;
				Minion *card;
				int var2 = 0;
				char arg3 = '0';
				iss>>var1;
				iss>>var2;
				iss>>arg3;
				if (var1 >= 1 && static_cast<unsigned>(var1)<=curPlayer->getBoardMinions().size()){
					card = (curPlayer->getBoardMinions())[var1-1];
					if (card->getActCost() > -1){
						bool validPlayer = true;
						Player *player = nullptr;
						if (var2 == 1) player = &player1;
						else if (var2 == 2) player = &player2;
						else if (var2 != 0) {
							cout <<"Not a valid player"<<endl;
							validPlayer = false;
						}
						if (validPlayer){
							if (card->getActCost()<=curPlayer->getCurMagic()){
								if (card->getAction() > 0){
									bool success = false;
									const int actCost = curPlayer->getCurMagic()-card->getActCost();
									if (player){
										if (static_cast<unsigned>(arg3 - '1') < player->getBoardMinions().size()){
											ActivatedAbility *ab = card->getActAbility();
											if (ab)	success = ab->doAbility(*player, arg3, card);
										}
										else cout <<"No such Minion"<<endl;
									} 
									else {
										ActivatedAbility *ab = card->getActAbility();
										if (ab)	success = ab->doAbility(*curPlayer, *nonActivePlayer, card);
									}
									if (success){
										curPlayer->setCurMagic(actCost);
										if (card->getDEF() > 0) card->setAction(card->getAction() - 1);
										handler.notifyObservers(curPlayer, nonActivePlayer);
									}else cout << "Invalid activated ability usage"<<endl;
								}else{
									cout<<"Not enough Action points"<<endl;
								} 
							}else{
								if (testing){
									curPlayer->setCurMagic(0);
									if (player) card->getActAbility()->doAbility(*player, arg3, card);
									else card->getActAbility()->doAbility(*curPlayer, *nonActivePlayer, card);
								}else cout<<"Not enough Mana to use"<<endl;
							}							
						}
					}else cout << card->getName() << " has no activated ability to use"<<endl;
				}
				else{
					cout<<"No such minion"<<endl;
				}
			
			}
			else if (command=="inspect"){
				int var = 0;
				iss>>var;
				if (var<=0 || static_cast<unsigned>(var)>curPlayer->getBoardMinions().size()){
					cout<<"No such card"<<endl;
				}
				else{
					board.display_minion(curPlayer->getBoardMinions()[var-1]);
				}

			}
			else if (command=="attack"){
				int var1 = 0;
				int var2 = 0;
				iss>>var1;
				iss>>var2;

				if (static_cast<unsigned>(var1) <= curPlayer->getBoardMinions().size()
					&& static_cast<unsigned>(var2) <= nonActivePlayer->getBoardMinions().size()){
					Minion *attackingMinion = curPlayer->getBoardMinions().at(var1 - 1);
					if (attackingMinion->getAction() <= 0){
						cout << "Not enough Action points"<<endl;
					}else{
						handler.minion_attack(attackingMinion, *nonActivePlayer, var2);
					}					
				}else cout<<"No such Minion"<<endl;
			}
			else if (command=="board"){
				board.display_board(nonActivePlayer,curPlayer);
			}
			else if (command=="hand"){
				board.display_hand(curPlayer);
			}

			else if (command=="draw"){
				if (testing){
					curPlayer->draw();
					handler.notifyObservers(BoardPlace::Hand, curPlayer);
				}
			}

			else if (command=="discard"){
				if (testing){
					int var = 0;
					iss>>var;
					if (var >= 1 && static_cast<unsigned>(var)<=curPlayer->getHand().size()){
						vector<Card*> &hand = curPlayer->getHand();
						handler.dumpCard(hand[var-1]);
						hand.erase(hand.begin()+var-1);
						handler.notifyObservers(BoardPlace::Hand, curPlayer);
					}
					else{
						cout<<"No such card"<<endl;
					}
				}
			}
			//cheat code starts
			//healing blast on you
			else if (command=="HealMeHealMe"){
				curPlayer->setHP(20);
				cout<<"You became healthy!!"<<endl;
				handler.notifyObservers(BoardPlace::Player,curPlayer);
			}
			//gain alot of mana
			else if (command=="IStarveForMana"){
				curPlayer->setCurMagic(999);
				cout<<"You feels the power flow inside you!"<<endl;
				handler.notifyObservers(BoardPlace::Player,curPlayer);
			}
			//Fill your board with novice pyromancers
			else if (command=="Pyromaniac"){
				int count=0;
				for (int i=0;i<5;++i){
					if ((unsigned)(i+1)>(curPlayer->getBoardMinions().size())){
						++count;
						Card *c=cardFactory.loadCard(*curPlayer, "Novice Pyromancer");
						c->play(handler, nullptr, '0');
						
					}
				}
				if (count==0){
					cout<<"Nobody showed up at you fire playing party :("<<endl;
				}
				else{
					cout<<count<<" novice pyromancer showed at the party, you will have trouble";
					cout<<" getting ride of them now."<<endl;
				}
				handler.notifyObservers(BoardPlace::Minions,curPlayer);

			}


			
		}
		handler.end_turn(*curPlayer, *nonActivePlayer);

		// change active player
		if (player2.isActive()){
			curPlayer=&player1;
			nonActivePlayer = &player2;
		}
		else{
			curPlayer=&player2;
			nonActivePlayer = &player1;
		}
		player1.setActive(!player1.isActive());
		player2.setActive(!player2.isActive());
	}
	delete xw;
}
