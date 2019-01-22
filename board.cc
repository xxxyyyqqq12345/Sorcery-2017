#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "board.h"
#include "ascii_graphics.h"
#include "player.h"
#include "ritual.h"
#include "spell.h"
#include "basic_minion.h"
#include "enchantment.h"
#include "window.h"
using namespace std;

void Board::attach_graphics(Xwindow *xw) {
	this->xw = xw;
}

void Board::display_hand_graphics(Player *p){	
	vector<Card*> &hand = p->getHand();
	for (unsigned int i=0;i<5;++i){
		if (i<hand.size()){
			hand[i]->draw_graphics(*xw, APHandX[i], APHandY);
		}
		else{
			xw->fillRectangle(APHandX[i], APHandY, Card::width, Card::height, 1);
		}
	}
}

void Board::draw_minion_graphics(Player *p){
	vector<Minion*> &board = p->getBoardMinions();
	const bool isActive = p->isActive();
	const int minY = isActive? APMinY: NAPMinY;
	const int *arr = isActive? APMinX: NAPMinX;
	for (unsigned int i=0;i<5;++i){
		if (i<board.size()){
			board[i]->draw_graphics(*xw, arr[i], minY);
		}
		else{
			this->xw->fillRectangle(arr[i], minY, Card::width, Card::height, 1);//<-to change
		}
	}
}

void Board::draw_ritual_graphics(Player *p){
	const bool isActive = p->isActive();
	const int *areaX = isActive? APAreaX: NAPAreaX;
	const int areaY = isActive? APAreaY: NAPAreaY;
	if (p->getRitual()!=nullptr){
		p->getRitual()->draw_graphics(*xw, areaX[aRitual], areaY);
	}
	else{
		this->xw->fillRectangle(areaX[aRitual], areaY, Card::width, Card::height, 1);//<-to change
	}
}

void Board::draw_grave_graphics(Player *p){
	vector<Minion*> &grave = p->getGrave();
	const bool isActive = p->isActive();
	const int *areaX = isActive? APAreaX: NAPAreaX;
	const int areaY = isActive? APAreaY: NAPAreaY;
	if (grave.size()==0){
		this->xw->fillRectangle(areaX[aGrave], areaY, Card::width, Card::height, 1);//<-to change
	}
	else{
		grave[grave.size()-1]->draw_graphics(*xw, areaX[aGrave], areaY);
	}
}

void Board::draw_player_graphics(Player *p){
	const bool isActive = p->isActive();
	const int *areaX = isActive? APAreaX: NAPAreaX;
	const int areaY = isActive? APAreaY: NAPAreaY;
	p->draw_graphics(*xw, areaX[aPlayer], areaY);
}

void Board::display_board_graphics(Player *p1,Player *p2){
	//draw minions
	draw_minion_graphics(p1);
	draw_minion_graphics(p2);

	// draw rituals
	draw_ritual_graphics(p1);
	draw_ritual_graphics(p2);

	// draw grave
	draw_grave_graphics(p1);
	draw_grave_graphics(p2);

	// draw players
	draw_player_graphics(p1);
	draw_player_graphics(p2);
}

void Board::display_board(Player *p1,Player *p2){
	//display acsii ritual
	vector<card_template_t> t1;
	if (p1->getRitual()!=nullptr){
		t1.push_back(display_ritual(p1->getRitual()->getName(),
			       p1->getRitual()->getCP(),
			       p1->getRitual()->getActCost(),
			       p1->getRitual()->getDescription(),
                               p1->getRitual()->getCharge()));
	}
	//display empty space and player
	else{t1.push_back(CARD_TEMPLATE_BORDER);}
		t1.push_back(CARD_TEMPLATE_EMPTY);
		t1.push_back(display_player_card(1,p1->getName(),p1->getHP(),p1->getCurMagic()));
		t1.push_back(CARD_TEMPLATE_EMPTY);
	//display ascii grave
	if (p1->getGrave().size()>=1){
		t1.push_back(p1->getGrave()[p1->getGrave().size()-1]->draw(this));
	}
	else{t1.push_back(CARD_TEMPLATE_BORDER);}
	
	for (unsigned int i=0;i<CARD_TEMPLATE_EMPTY.size();++i){
		cout<<t1[0][i]<<t1[1][i]<<t1[2][i]<<t1[3][i]<<t1[4][i]<<endl;
	}
	//display acsii board minions
	vector<card_template_t> t2;
	for (unsigned int i=0;i<=4;++i){
		if (i<p1->getBoardMinions().size()){
			t2.push_back(p1->getBoardMinions()[i]->draw(this));

		}

		else{
			t2.push_back(CARD_TEMPLATE_BORDER);

		}
		
	}
	for (unsigned int i=0;i<CARD_TEMPLATE_EMPTY.size();++i){
		cout<<t2[0][i]<<t2[1][i]<<t2[2][i]<<t2[3][i]<<t2[4][i]<<endl;
	}
	//display center graphic
	for (unsigned int i=0;i<CENTRE_GRAPHIC.size();++i){
		cout<<CENTRE_GRAPHIC[i]<<endl;
	}
	//display acsii board minions
	vector<card_template_t> t3;
	for (unsigned int i=0;i<=4;++i){
		if (i<p2->getBoardMinions().size()){
			t3.push_back(p2->getBoardMinions()[i]->draw(this));

		}

		else{
			t3.push_back(CARD_TEMPLATE_BORDER);

		}
		
	}
	for (unsigned int i=0;i<CARD_TEMPLATE_EMPTY.size();++i){
		cout<<t3[0][i]<<t3[1][i]<<t3[2][i]<<t3[3][i]<<t3[4][i]<<endl;
	}
	//display acsii ritual
	vector<card_template_t> t4;
	if (p2->getRitual()!=nullptr){
		t4.push_back(display_ritual(p2->getRitual()->getName(),
			       p2->getRitual()->getCP(),
			       p2->getRitual()->getActCost(),
			       p2->getRitual()->getDescription(),
                               p2->getRitual()->getCharge()));
	}
	//display empty space and player
	else{t4.push_back(CARD_TEMPLATE_BORDER);}
		t4.push_back(CARD_TEMPLATE_EMPTY);
		t4.push_back(display_player_card(1,p2->getName(),p2->getHP(),p2->getCurMagic()));
		t4.push_back(CARD_TEMPLATE_EMPTY);
	//display ascii grave
	if (p2->getGrave().size()>=1){
		t4.push_back(p2->getGrave()[p2->getGrave().size()-1]->draw(this));
	}
	else{t4.push_back(CARD_TEMPLATE_BORDER);}
	
	for (unsigned int i=0;i<CARD_TEMPLATE_EMPTY.size();++i){
		cout<<t4[0][i]<<t4[1][i]<<t4[2][i]<<t4[3][i]<<t4[4][i]<<endl;
	}
	
}


void Board::display_hand(Player *p){
	vector<card_template_t> t1;
	for (unsigned int i=0;i<=4;++i){
		if (i<p->getHand().size()){
			t1.push_back(p->getHand()[i]->draw(this));

		}

		else{
			t1.push_back(CARD_TEMPLATE_BORDER);

		}

	}
	for (unsigned int i=0;i<CARD_TEMPLATE_EMPTY.size();++i){
		cout<<t1[0][i]<<t1[1][i]<<t1[2][i]<<t1[3][i]<<t1[4][i]<<endl;
	}

}
void Board::display_minion(Minion *m){
	card_template_t minion=m->draw(this);
	for (unsigned int i=0;i<CARD_TEMPLATE_EMPTY.size();++i){
		cout<<minion[i]<<endl;
	}
	vector<card_template_t> t1=m->draw_self(this);
	if(t1.size()==0){return;}
	else{
		int processed=0;
		int in_process;
		while (1){
			in_process=min(static_cast<int>(t1.size())-processed,5);
			for (unsigned int j=0;j<CARD_TEMPLATE_EMPTY.size();++j){
				for (int l=processed;l<processed+in_process;++l){
					cout<<t1[l][j];
				}
				cout<<endl;
			}
			processed=processed+in_process;
			if (static_cast<unsigned>(processed)>=t1.size()){
				break;
			}
			
		}
	}
}

card_template_t Board::draw_minion_ne(BasicMinion *m){
	return display_minion_no_ability(m->getName(),m->getCP(),m->getATK(),m->getDEF());
}
card_template_t Board::draw_minion_ae(BasicMinion *m){
	return display_minion_activated_ability(m->getName(),m->getCP(),m->getATK(),m->getDEF(),m->getActCost(),m->getDescription());
}
card_template_t Board::draw_minion_te(BasicMinion *m){
	return display_minion_triggered_ability(m->getName(),m->getCP(),m->getATK(),m->getDEF(),m->getDescription());
}
card_template_t Board::draw_ritual(Ritual *r){
	return display_ritual(r->getName(),r->getCP(),r->getActCost(),r->getDescription(),r->getCharge());
}
card_template_t Board::draw_spell(Spell *s){
	return display_spell(s->getName(),s->getCP(),s->getDescription());
}
card_template_t Board::draw_enchantment(Enchantment *e){
	return display_enchantment(*(e->getEnName()),e->getCP(),e->getDescription());
}
card_template_t Board::draw_enchantment_a_d(Enchantment *e){
	return display_enchantment_attack_defence(*(e->getEnName()),e->getCP(),e->getDescription(),e->getAtkRep(),e->getDefRep());
}

void Board::notify(Player *AP,Player *NAP){
	if (xw){
		display_board_graphics(AP,NAP);
		display_hand_graphics(AP);
	}
}
void Board::notify(BoardPlace bp, Player *p){
	if (xw){
		if (bp == BoardPlace::Ritual) draw_ritual_graphics(p);
		else if (bp == BoardPlace::Minions) draw_minion_graphics(p);
		else if (bp == BoardPlace::Hand) display_hand_graphics(p);
		else if (bp == BoardPlace::Grave) draw_grave_graphics(p);
		else if (bp == BoardPlace::Player) draw_player_graphics(p);	
	}
}
