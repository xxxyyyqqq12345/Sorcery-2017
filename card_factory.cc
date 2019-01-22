#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "card_factory.h"
#include "minion.h"
#include "spell.h"
#include "ability.h"
#include "activated_ability.h"
#include "triggered_ability.h"
#include "ritual.h"
#include "enchantment.h"

using namespace std;

CardFactory::CardFactory(Handler &h): h{h} {}

bool dealDmgToMinion(Handler &h, Player &target, int cardIndex, int dmg){
	vector<Minion*> &boardMinions = target.getBoardMinions();
	if (boardMinions.size() == 0 
		|| boardMinions.size() - 1 < static_cast<unsigned>(cardIndex)) {
		cout << "No minion at position " << cardIndex + 1 << " for " << target.getName() <<endl; 
		return false;
	}
	Minion *m = boardMinions.at(cardIndex);
	m->setDEF(m->getDEF() - dmg);
	h.moveToGrave(target, cardIndex);
	return true;
}

void dealDmgToMinion(Handler &h, Minion &m, int dmg){
	m.setDEF(m.getDEF() - dmg);
	h.moveToGrave(m);
}

void changeMinionStats(Minion &m, int atkInc, int defInc){
	m.setATK(m.getATK() + atkInc);
	m.setDEF(m.getDEF() + defInc);
}

bool summonAirEle(Handler &h, Player &p, int num){
	vector<Minion*> &board = p.getBoardMinions();
	if (board.size() < 5){
		while (num > 0 && board.size() < 5){
			CardFactory cf{h};
			Minion *m = dynamic_cast<Minion*>(cf.loadCard(p, "Air Elemental"));
			board.push_back(m);
			h.minion_enters_play(p, *m);
			--num;
		}
		return true;
	}else return false;
}

bool boneGolem(Handler &h
 		, Minion &trigHolder, Minion *mTarget){
	changeMinionStats(trigHolder, 1, 1);
	return true;
}

bool fireElemental(Handler &h
 		, Minion &trigHolder, Minion *mTarget){
	if (&trigHolder.getOwner() != &mTarget->getOwner())
		dealDmgToMinion(h, *mTarget, 1);
	return true;
}

bool potionSeller(Handler &h
 		, Minion &trigHolder, Minion *mTarget){
	Player &p = trigHolder.getOwner();
	vector<Minion*> &board = p.getBoardMinions();
	for (auto &minion: board){
		changeMinionStats(*minion, 0, 1);
	}
	return true;
}

bool novicePyromancer(Handler &h, Player &target, char c){
	if (c >= '1' && c <= '5'){
		return dealDmgToMinion(h, target, c-'1', 1);
	}
	return false;
}

bool apprenticeSummoner(Handler &h
 		, Player &activeP, Player &nonActiveP){
	return summonAirEle(h, activeP, 1);
}

bool masterSummoner(Handler &h
 		, Player &activeP, Player &nonActiveP){
	return summonAirEle(h, activeP, 3);
}

bool banish(Handler &h, Player &target, char c){
	if (c >= '1' && c <= '5'){
		vector<Minion*> &boardMinions = target.getBoardMinions();
		if (boardMinions.size() == 0 
			|| boardMinions.size() - 1 < static_cast<unsigned>(c-'1')) {
			cout << "No minion at position " << c << " for " << target.getName() <<endl; 
			return false;
		}
		Minion *m = boardMinions.at(c - '1');
		if (m == nullptr) cout << "Target required" << endl;
		else {
			boardMinions.erase(boardMinions.begin() + (c - '1'));
			target.getGrave().push_back(h.minion_leaves_play(target, *m));
		}
	}
	else if (c == 'r'){
		Ritual *rit = target.getRitual();
		if (rit == nullptr){
			cout << target.getName() << " has no ritual in play"<<endl;
			return false;
		}else{
			target.setRitual(nullptr);
		}
	}else{
		cout << "Invalid command" << endl;
		return false;
	}
	return true;
}

bool unsummon(Handler &h, Player &target, char c){
	if (c >= '1' && c <= '5'){
		vector<Minion*> &boardMinions = target.getBoardMinions();
		if (boardMinions.size() == 0 
			|| boardMinions.size() - 1 < static_cast<unsigned>(c-'1')) {
			cout << "No minion at position " << c << " for " << target.getName() <<endl; 
			return false;
		}
		Minion *m = boardMinions.at(c - '1');
		m = h.minion_leaves_play(target, *m);
		if (target.getHand().size() < 5) target.getHand().push_back(m);
		else h.dumpCard(m);
		boardMinions.erase(boardMinions.begin() + (c - '1'));
	}else {
		cout << "Invalid command" << endl;
		return false;
	}
	return true;
}

bool disenchant(Handler &h, Player &target, char c){
	if (c >= '1' && c <= '5'){
		vector<Minion*> &boardMinions = target.getBoardMinions();
		if (boardMinions.size() == 0 
			|| boardMinions.size() - 1 < static_cast<unsigned>(c-'1')) {
			cout << "No minion at position " << c << " for " << target.getName() <<endl; 
			return false;
		}
		Minion *m = boardMinions.at(c - '1');
		Minion *e = m->detach();
		if (e){
			boardMinions[c- '1'] = e;
			delete m;
		}else{
			cout << m->getName() << " has no enchantments"<<endl;
		}
	}else {
		cout << "Invalid command" << endl;
		return false;
	}
	return true;
}

bool recharge(Handler &h
 		, Player &player1, Player &player2){
	Player &target = player1.isActive()? player1: player2;
	Ritual *rit = target.getRitual();
	if (rit == nullptr){
		cout << target.getName() << " has no ritual in play"<<endl;
		return false;
	}else{
		rit->setCharge(rit->getCharge() + 3);
		return true;
	}
}

bool raiseDead(Handler &h
 		, Player &player1, Player &player2){
	Player &target = player1.isActive()? player1: player2;
	vector<Minion*> &boardMinions = target.getBoardMinions();
	vector<Minion*> &graveyard = target.getGrave();
	if (graveyard.size() == 0){
		cout << "Empty graveyard." << endl;
		return false;
	}
	if (boardMinions.size() >= 5){
		cout << "Full board of minions." << endl;
		return false;
	}
	Minion *m = graveyard.back();
	boardMinions.push_back(m);
	m->setDEF(1);
	graveyard.pop_back();
	h.minion_enters_play(target, *m);
	return true;
}

bool blizzard(Handler &h
 		, Player &player1, Player &player2){
	Player *player = player1.isActive()? &player1: &player2;
	vector<Minion*> *board = &(player->getBoardMinions());
	// deal dmg to active player minions
	unsigned int size = board->size();
	for (unsigned int i = 0; i < size; ++i){
		dealDmgToMinion(h, *player, i, 2);
		if (board->size() < size){
			--i;
			--size;
		}
	}
	
	player = !(player1.isActive())? &player1: &player2;
	board = &(player->getBoardMinions());
	// deal dmg to non-active player minions
	size = board->size();
	for (unsigned int i = 0; i < size; ++i){
		dealDmgToMinion(h, *player, i, 2);
		if (board->size() < size){
			--i;
			--size;
		}
	}
	return true;
}

bool darkRitual(Handler &h
 		, Player &active, Player &nonActive){
	active.setCurMagic(active.getCurMagic() + 1);
	return true;
}

bool auraOfPower(Handler &h, Player &p, Minion &m){
	if (&(m.getOwner()) == &p){
		changeMinionStats(m, 1, 1);
		return true;
	}else return false;
}

bool standstill(Handler &h, Player &p, Minion &m){
	if (m.isOnBoard()){
		Player &owner = m.getOwner();
		vector<Minion*> &boardMinions = owner.getBoardMinions();
		owner.getGrave().push_back(&m);
		boardMinions.pop_back();
		h.minion_leaves_play(m.getOwner(), m);
	}
	return true;
}

void enrage(Minion *pm, Enchantment *en) {
	en->setATK(pm->getATK());
	en->setDEF(pm->getDEF());
}

bool haste(Handler &h
 		, Minion &trigHolder, Minion *mTarget) {
	trigHolder.setAction(trigHolder.getAction() + 1);
	return true;
}

void magicFatigue(Minion *pm, Enchantment *en){
	en->setActCost(2);
}

bool silence(Handler &h
 		, Minion *m){
	cout << m->getName() << " is silenced"<<endl;
	return false;
}

Card* CardFactory::loadCard(Player &p, const string &cardName) {
	// Minions
	if (cardName == "Air Elemental") {
		Minion *M = new BasicMinion{1, 1};
		M->setCP(0);
		M->setDescription("");
		M->setName(cardName);
		M->setOwner(&p);
		return M;
	}
	else if (cardName == "Earth Elemental") {
		Minion *M = new BasicMinion{4, 4};
		M->setCP(3);
		M->setDescription("");
		M->setName(cardName);
		M->setOwner(&p);
		return M;
	}
	else if (cardName == "Bone Golem"){
		Minion *M = new BasicMinion{1, 3};
		TriggeredAbility *ta = new TriggeredAbility(h, boneGolem, TrigType::MinionLeave);
		M->addTrigAbility(ta);
		M->setCP(2);
		M->setDescription("Gain +1/+1 whenever a minion leaves play.");
		M->setName(cardName);
		M->setOwner(&p);
		return M;
	}
	else if (cardName == "Fire Elemental"){
		Minion *M = new BasicMinion{2, 2};
		TriggeredAbility *ta = new TriggeredAbility(h, fireElemental, TrigType::MinionEnter);
		M->addTrigAbility(ta);
		M->setCP(2);
		M->setDescription("Whenever an opponent's minion enters play, deal 1 damage to it.");
		M->setName(cardName);
		M->setOwner(&p);
		return M;
	}	
	else if (cardName == "Potion Seller"){
		Minion *M = new BasicMinion{1, 3};
		TriggeredAbility *ta = new TriggeredAbility(h, potionSeller, TrigType::EndTurn);
		M->addTrigAbility(ta);
		M->setCP(2);
		M->setDescription("At the end of your turn, all your minions gain +0/+1.");
		M->setName(cardName);
		M->setOwner(&p);
		return M;
	}
	else if (cardName == "Novice Pyromancer"){
		Minion *M = new BasicMinion{0, 1};
		ActivatedAbility *aa = new ActivatedAbility(h, novicePyromancer);
		M->setActAbility(aa);
		M->setActCost(1);
		M->setCP(1);
		M->setDescription("Deal 1 damage to target minion");
		M->setName(cardName);
		M->setOwner(&p);
		return M;
	}
	else if (cardName == "Apprentice Summoner"){
		Minion *M = new BasicMinion{1, 1};
		ActivatedAbility *aa = new ActivatedAbility(h, apprenticeSummoner);
		M->setActAbility(aa);
		M->setActCost(1);
		M->setCP(1);
		M->setDescription("Summon a 1/1 air elemental");
		M->setName(cardName);
		M->setOwner(&p);
		return M;
	}
	else if (cardName == "Master Summoner"){
		Minion *M = new BasicMinion{2, 3};
		ActivatedAbility *aa = new ActivatedAbility(h, masterSummoner);
		M->setActAbility(aa);
		M->setActCost(2);
		M->setCP(3);
		M->setDescription("Summon up to three 1/1 air elementals");
		M->setName(cardName);
		M->setOwner(&p);
		return M;
	}
	// Spells
	else if (cardName == "Banish"){
		ActivatedAbility *aa = new ActivatedAbility(h, banish);
		Spell *sp = new Spell{aa};
		sp->setCP(2);
		sp->setDescription("Destroy target minion or ritual");
		sp->setName(cardName);
		sp->setOwner(&p);
		return sp;
	}
	else if (cardName == "Unsummon"){
		ActivatedAbility *aa = new ActivatedAbility(h, unsummon);
		Spell *sp = new Spell{aa};
		sp->setCP(1);
		sp->setDescription("Return target minion to its owner's hand");
		sp->setName(cardName);
		sp->setOwner(&p);
		return sp;
	}	
	else if (cardName == "Recharge"){
		ActivatedAbility *aa = new ActivatedAbility(h, recharge);
		Spell *sp = new Spell{aa};
		sp->setCP(1);
		sp->setDescription("Your ritual gains 3 charges");
		sp->setName(cardName);
		sp->setOwner(&p);
		return sp;
	}
	else if (cardName == "Raise Dead"){
		ActivatedAbility *aa = new ActivatedAbility(h, raiseDead);
		Spell *sp = new Spell{aa};
		sp->setCP(1);
		sp->setDescription("Resurrect the top minion in your graveyard and set its defence to 1");
		sp->setName(cardName);
		sp->setOwner(&p);
		return sp;
	}
	else if (cardName == "Blizzard"){
		ActivatedAbility *aa = new ActivatedAbility(h, blizzard);
		Spell *sp = new Spell{aa};
		sp->setCP(3);
		sp->setDescription("Deal 2 damage to all minions");
		sp->setName(cardName);
		sp->setOwner(&p);
		return sp;	
	}
	else if (cardName == "Disenchant"){
		ActivatedAbility *aa = new ActivatedAbility(h, disenchant);
		Spell *sp = new Spell{aa};
		sp->setCP(1);
		sp->setDescription("Destroy the top enchantment on target minion");
		sp->setName(cardName);
		sp->setOwner(&p);
		return sp;	
	}

	// Rituals
	else if (cardName == "Dark Ritual"){
		TriggeredAbility *ta = new TriggeredAbility(h, darkRitual, TrigType::StartTurn);
		Ritual *rit = new Ritual{ta, 5, 1};
		rit->setCP(0);
		rit->setCharge(5);
		rit->setDescription("At the start of your turn, gain 1 magic");
		rit->setName(cardName);
		rit->setOwner(&p);
		return rit;	
	}
	else if (cardName == "Aura of Power"){
		TriggeredAbility *ta = new TriggeredAbility(h, auraOfPower, TrigType::MinionEnter);
		Ritual *rit = new Ritual{ta, 4, 1};
		rit->setCP(1);
		rit->setDescription("Whenever a minion enters play under your control, it gains +1/+1");
		rit->setName(cardName);
		rit->setOwner(&p);
		return rit;
	}
	else if (cardName == "Standstill"){
		TriggeredAbility *ta = new TriggeredAbility(h, standstill, TrigType::MinionEnter);
		Ritual *rit = new Ritual{ta, 4, 2};
		rit->setCP(3);
		rit->setDescription("Whenever a minion enters play, destroy it");
		rit->setName(cardName);
		rit->setOwner(&p);
		return rit;
	}

  // Enchantments
	else if (cardName == "Giant Strength") {
		Enchantment *en{new Enchantment(2, 2)};
		en->setAtkRep("+2");
		en->setDefRep("+2");
		en->setCP(1);
		en->setDescription("");
		en->setName(cardName);
		en->setEnName(cardName);
		en->setOwner(&p);
		return en;
	}
	else if (cardName == "Enrage") {
		Enchantment *en{new Enchantment(enrage)};
		en->setAtkRep("*2");
		en->setDefRep("*2");
		en->setCP(2);
		en->setDescription("");
		en->setName(cardName);
		en->setEnName(cardName);
		en->setOwner(&p);
		return en;
	}
	else if (cardName == "Haste") {
		TriggeredAbility *ta = new TriggeredAbility(h, haste, TrigType::StartTurn);
		Enchantment *en{new Enchantment(ta)};
		en->setCP(1);
		en->setDescription("Enchanted minion gains +1 action each turn");
		en->setName(cardName);
		en->setEnName(cardName);
		en->setOwner(&p);
		return en;
	}
	else if (cardName == "Magic Fatigue") {
		Enchantment *en{new Enchantment(magicFatigue)};
		en->setCP(0);
		en->setDescription("Enchanted minion's activated ability costs 2 more");
		en->setName(cardName);
		en->setEnName(cardName);
		en->setOwner(&p);
		return en;
	}
	else if (cardName == "Silence") {
		ActivatedAbility *aa = new ActivatedAbility(h, silence);
		Enchantment *en{new Enchantment(aa)};
		en->setCP(1);
		en->setDescription("Enchanted minion cannot use abilities");
		en->setName(cardName);
		en->setEnName(cardName);
		en->setOwner(&p);
		return en;
	}

	else { return nullptr; }
}


void CardFactory::loadDeck(Player &p, const string &deckFile) {
	vector<Card*> &deck = p.getDeck();
	ifstream theDeck {deckFile};
	string cardName;
	while (getline(theDeck, cardName)) {
		Card* c = loadCard(p, cardName);
		deck.push_back(c);
	}
}
