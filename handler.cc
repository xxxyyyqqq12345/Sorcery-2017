#include "handler.h"
#include "spell.h"
#include "ability.h"
#include "activated_ability.h"
#include "triggered_ability.h"
#include "enchantment.h"

#include <vector>

using namespace std;

Handler::Handler(Player *p1, Player *p2): p1{p1}, p2{p2} {}

bool Handler::card_played(Ritual &R){
	R.getOwner().setRitual(&R);
	notifyObservers(BoardPlace::Ritual, &R.getOwner());
	return true;
}

bool Handler::card_played(Spell &S, Player *target, char targetCard){
	bool result = true;
	if (target == nullptr){
		result = S.doAbility(*p1, *p2);
	}else{
		result = S.doAbility(*target, targetCard);
	}
	if (result){
		dumpCard(&S);
		if (p1->isActive())	notifyObservers(p1, p2);
		else notifyObservers(p1, p2);
	}
	return result;
}

bool Handler::card_played(Minion &M, Player *target, char targetCard){
	Player &mOwner = M.getOwner();
	vector<Minion*> &board = mOwner.getBoardMinions();
	if (board.size() < 5) {
		board.push_back(&M);
		minion_enters_play(mOwner, M);
		return true;
	}
	else{
		cout << "Full board of minions"<<endl;
		return false;
	}	
}

bool Handler::card_played(Enchantment &E, Player *target, char targetCard) {
	if (targetCard >= '1' && targetCard <= '5') {
		vector<Minion*> &boardMinions = target->getBoardMinions();
		Minion *m = boardMinions.at(targetCard - '1');
		m = E.attach(m);
		target->getBoardMinions()[targetCard - '1'] = m;
		notifyObservers(BoardPlace::Minions, target);
		return true;
	}
	else return false;
}


void Handler::checkRitualsFromMinionTrig(Player &p, Minion &m, TrigType tt){
	if (tt == TrigType::MinionEnter || tt == TrigType::MinionLeave){
		Player *AP;
		Player *NAP;
		if (p1->isActive()){
			AP = p1;
			NAP = p2;
		}else{
			AP = p2;
			NAP = p1;
		}
		Ritual *rit = AP->getRitual();
		if (rit != nullptr) {
			if (rit->getCondition() == tt
				&& rit->getCharge() >= rit->getActCost()){
				rit->doAbility(*AP, m);
				notifyObservers(AP, NAP);
			}
		}

		rit = NAP->getRitual();
		if (rit != nullptr) {
			if (rit->getCondition() == tt 
				&& rit->getCharge() >= rit->getActCost()){
				rit->doAbility(*NAP, m);
				notifyObservers(AP, NAP);
			}
		}
	}
}

void Handler::checkMinions(Minion &m, TrigType tt){
	Player *AP;
	Player *NAP;
	if (p1->isActive()){
		AP = p1;
		NAP = p2;
	}else{
		AP = p2;
		NAP = p1;
	}
	vector<Minion*> *board = &(AP->getBoardMinions());
	for (auto &minion : *board){
		auto tas = minion->getTrigAbilities();
		for (auto &ta: *tas){
			if (ta && ta->getCondition() == tt){
				ta->doAbility(*minion, &m);
				notifyObservers(AP, NAP);
			}
		}
	}

	board = &(NAP->getBoardMinions());
	for (auto &minion : *board){
		auto tas = minion->getTrigAbilities();
		for (auto &ta: *tas){
			if (ta && ta->getCondition() == tt){
				ta->doAbility(*minion, &m);
				notifyObservers(AP, NAP);
			}
		}
	}
}

void Handler::checkMinions(Player &player, TrigType tt){
	Player *AP;
	Player *NAP;
	if (p1->isActive()){
		AP = p1;
		NAP = p2;
	}else{
		AP = p2;
		NAP = p1;
	}
	vector<Minion*> *board = &(player.getBoardMinions());
	for (auto &minion : *board){
		auto tas = minion->getTrigAbilities();
		for (auto &ta: *tas){
			if (ta && ta->getCondition() == tt){
				ta->doAbility(*minion, nullptr);
				notifyObservers(AP, NAP);
			}
		}
	}
}

void Handler::start_turn(Player &active, Player &nonActive){

	// give action points
	vector<Minion*> &board = active.getBoardMinions();
	for (auto &m: board){
		m->setAction(1);
	}

	// check rituals
	Ritual *rit = active.getRitual();
	if (rit != nullptr) {
		if (rit->getCondition() == TrigType::StartTurn 
			&& rit->getCharge() >= rit->getActCost()){
			rit->doAbility(active, nonActive);
		}
	}

	// check minions
	checkMinions(active, TrigType::StartTurn);
	notifyObservers(&active, &nonActive);
}

void Handler::end_turn(Player &active, Player &nonActive){
	// check ritual
	Ritual *rit = active.getRitual();
	if (rit != nullptr) {
		if (rit->getCondition() == TrigType::EndTurn 
			&& rit->getCharge() >= rit->getActCost()){
			rit->doAbility(active, nonActive);
		}
	}

	// check minions
	checkMinions(active, TrigType::EndTurn);
}

void Handler::minion_enters_play(Player &p, Minion &m){
	notifyObservers(BoardPlace::Minions, &p);
	m.setOnBoard(true);
	m.setAction(0);
	checkRitualsFromMinionTrig(p, m, TrigType::MinionEnter);
	checkMinions(m, TrigType::MinionEnter);
}

Minion *Handler::minion_leaves_play(Player &p, Minion &m){
	notifyObservers(BoardPlace::Minions, &p);
	m.setOnBoard(false);
	checkRitualsFromMinionTrig(p, m, TrigType::MinionLeave);
	checkMinions(m, TrigType::MinionLeave);

	// remove enchantments
	Enchantment *e=dynamic_cast<Enchantment*>(&m);
	if (e){
		Minion *toDelete= e;
		while (1){
			Minion *parent =toDelete->detach();
			if (parent == nullptr){
				return toDelete;
			}
			else{
				dumpCard(toDelete);
			}
				
			toDelete = parent;
		}
	}
	return &m;
}

Minion *Handler::moveToGrave(Player &p, int pCard){
	vector<Minion*> &boardMinions = p.getBoardMinions();
	Minion *m = boardMinions.at(pCard);
	if (m->getDEF() <= 0){
		cout << p.getName() << "'s " << m->getName() << " destroyed" << endl;
		boardMinions.erase(boardMinions.begin() + (pCard));
		m = minion_leaves_play(p, *m);
		p.getGrave().push_back(m);
		notifyObservers(BoardPlace::Grave, &p);
	}
	return m;
}

Minion *Handler::moveToGrave(Minion &m){
	Minion *minion = &m;
	if (m.getDEF() <= 0){
		Player &p = m.getOwner();
		cout << p.getName() << "'s " << m.getName() << " destroyed" << endl;
		
		vector<Minion*> &board = p.getBoardMinions();
		for (unsigned int i = 0; i < board.size(); ++i){
			if (board.at(i) == &m) board.erase(board.begin() + i);
		}
		minion = minion_leaves_play(p, m);
		p.getGrave().push_back(minion);
		notifyObservers(BoardPlace::Grave, &minion->getOwner());
	}
	return minion;
}


void Handler::minion_attack(Minion *attackingMinion, Player &targetP, int targetCard){
	if (targetCard == 0){
		targetP.setHP(targetP.getHP() - attackingMinion->getATK());
		notifyObservers(BoardPlace::Player, &targetP);
	}else{
		Minion *attackedMinion = targetP.getBoardMinions().at(targetCard - 1);
		Player *AP = &attackingMinion->getOwner();

		// damage calculation
		attackingMinion->setDEF(attackingMinion->getDEF() - attackedMinion->getATK());
		attackedMinion->setDEF(attackedMinion->getDEF() - attackingMinion->getATK());

		// move to graveyard if needed
		attackingMinion = moveToGrave(*attackingMinion);
		attackedMinion = moveToGrave(*attackedMinion);

		notifyObservers(BoardPlace::Minions, AP);
		notifyObservers(BoardPlace::Minions, &targetP);
	}

	attackingMinion->setAction(attackingMinion->getAction() - 1);
}

void Handler::dumpCard(Card *c){
	cardDump.push_back(c);
}

Handler::~Handler(){
	for (auto &s: cardDump) delete s;
}
