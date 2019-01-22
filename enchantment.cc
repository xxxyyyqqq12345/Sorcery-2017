#include "enchantment.h"
#include "handler.h"
#include "minion.h"
#include "activated_ability.h"
#include "triggered_ability.h"
#include "window.h"

class BasicMinion;

using namespace std;

Enchantment::Enchantment(void(*applyEffect)(Minion *pm, Enchantment *rm)): applyEffect{applyEffect} {}

Enchantment::Enchantment(int atk, int def): Minion{atk,def} {}

Enchantment::Enchantment(TriggeredAbility* trigAbility): trigAbility{trigAbility} {}

Enchantment::Enchantment(ActivatedAbility *actAbility): Minion{actAbility} {}

void Enchantment::draw_graphics(Xwindow &xw, const int x, const int y){
	stringstream ss;
	Minion *coreMinion = find_basic_minion();
	xw.fillRectangle(x, y, width, height);
	// cost barrier
	if (coreMinion) ss<<coreMinion->getCP();
	else ss << getCP();
	xw.fillRectangle(x + width - separator - 5, y, 1, separator, Xwindow::Blue);
	xw.drawString(x + width - separator - 5 + separator/2, y + separator/2 + 3, ss.str());
	// card name
	xw.fillRectangle(x, y + separator, width, 1, Xwindow::Blue);
	xw.drawString(x + separator/2, y + separator - separator/2 + 3, getName());
	// card type
	string type = coreMinion? "Minion": "Enchantment";
	xw.fillRectangle(x, y + separator * 2, width, 1, Xwindow::Blue);
	xw.drawString(x + separator/2, y + separator * 2 - separator/2 + 3 * 2, type);

	int aaPadding = 0;
	int aaMaxChar = 0;
	if (coreMinion && coreMinion->getActCost() > -1){
		// activated ability cost box
		xw.fillRectangle(x, y + separator * 3, separator, 1, Xwindow::Blue);
		xw.fillRectangle(x + separator, y + separator * 2, 1, separator, Xwindow::Blue);
		ss.str(string());
		ss << getActCost();
		xw.drawString(x - 5 + separator/2, y + separator*2 + separator/2 + 3, ss.str());
		aaPadding = 30;
		aaMaxChar = 5;
	}
	// description
	string des = coreMinion? coreMinion->getDescription(): getDescription();
	unsigned int count = 0;
	int deltaY = 0;
	while (count < des.size()){
		xw.drawString(x + separator/2 - 3 + aaPadding
			, y + separator * 3 - separator/2 + 3 * 2 + deltaY
			, des.substr(count, max_char - aaMaxChar));
		deltaY += 13;
		count += max_char - aaMaxChar;
	}
	if ((getAtkRep() != "" && getDefRep() != "")
		|| coreMinion){
		string atkStr, defStr;
		if (coreMinion){
			ss.str(std::string());
			ss << getATK();
			atkStr = ss.str();
			ss.str(std::string());
			ss << getDEF();
			defStr = ss.str();
		}else{
			atkStr = getAtkRep();
			defStr = getDefRep();
		}
		// atk box
		xw.fillRectangle(x, height + y - separator, separator, 1, Xwindow::Blue);
		xw.fillRectangle(x + separator, height + y - separator, 1, separator, Xwindow::Blue);
		xw.drawString(x + separator/2 - 4, y + height - separator + separator/2 + 3, atkStr);
		// def box
		xw.fillRectangle(width + x - separator, height + y - separator, separator, 1, Xwindow::Blue);
		xw.fillRectangle(width + x - separator, height + y - separator, 1, separator, Xwindow::Blue);
		xw.drawString(x + width - separator + separator/2 - 4, y + height - separator + separator/2 + 3, 
			defStr);
	}
}

int Enchantment::getATK() {
	if (parentMinion)	return parentMinion->getATK() + atk;
	else return atk;
}

int Enchantment::getDEF() {
	if (parentMinion) return parentMinion->getDEF() + def;
	else return def;
}

string &Enchantment::getAtkRep(){
	return atkRep;
}

string &Enchantment::getDefRep(){
	return defRep;
}

string *Enchantment::getEnName(){
	return &enName;
}

void Enchantment::setATK(int atk) {
	if (parentMinion){
		if (atk <= parentMinion->getATK()){
			parentMinion->setATK(atk);
			this->atk = 0;
		}
		else if (atk <= defaultAtk + parentMinion->getATK()){
			this->atk = atk - parentMinion->getATK();
		}else{
			parentMinion->setATK(atk - this->atk);
		}
	}else{
		this->atk = this->defaultAtk = atk;
	}
}
	
void Enchantment::setDEF(int def) {
	if (parentMinion){
		if (def <= parentMinion->getDEF()){
			parentMinion->setDEF(def);
			this->def = 0;
		}
		else if (def <= defaultDef + parentMinion->getDEF()){
			this->def = def - parentMinion->getDEF();
		}else{
			parentMinion->setDEF(def - this->def);
		}
	}else{
		this->def = this->defaultDef = def;
	}
}


void Enchantment::setAtkRep(string atkRep){
	this->atkRep = atkRep;
}

void Enchantment::setDefRep(string defRep){
	this->defRep = defRep;
}

void Enchantment::setParent(Minion *m) {
	parentMinion = m;
}

void Enchantment::setEnName(string enName){
	this->enName = enName;
}

vector<TriggeredAbility*> *Enchantment::getTrigAbilities() {
	if (parentMinion) return parentMinion->getTrigAbilities();
	return nullptr;
}

Enchantment* Enchantment::attach(Minion *m) {
	if (applyEffect) applyEffect(m, this);
	setParent(m);
	if (trigAbility) {
		parentMinion->getTrigAbilities()->push_back(trigAbility);
		trigAbility->doAbility(*parentMinion, nullptr);
	}
	Player &p = m->getOwner();
	setOwner(&p);
	string name = m->getName();
	setName(name);
	return this;
}

Minion* Enchantment::detach() {
	return parentMinion;
}

bool Enchantment::play(Handler &h, Player *target
								, char targetCard){
	return h.card_played(*this, target, targetCard);
}

int Enchantment::getActCost(){
	return parentMinion->getActCost() + actCost;
}

void Enchantment::setActCost(int actCost){
	this->actCost = actCost;
}

ActivatedAbility *Enchantment::getActAbility(){
	if (!actAbility) return parentMinion->getActAbility();
	else return actAbility;
}

void Enchantment::addTrigAbility(TriggeredAbility *ta){
	trigAbility = ta;
}

void Enchantment::setActAbility(ActivatedAbility *aa){
	actAbility = aa;
}

int Enchantment::getAction(){
	return parentMinion->getAction();
}

void Enchantment::setAction(int action){
	parentMinion->setAction(action);
}

card_template_t Enchantment::draw(Board *b){
	if (this->find_basic_minion()){
		int t_A=this->find_basic_minion()->getATK();
		int t_D=this->find_basic_minion()->getDEF();
		int act_cost=this->find_basic_minion()->getActCost();
		this->find_basic_minion()->setATK(this->getATK());
		this->find_basic_minion()->setDEF(this->getDEF());
		this->find_basic_minion()->setActCost(this->getActCost());
		card_template_t result=this->find_basic_minion()->draw(b);
		this->find_basic_minion()->setATK(t_A);
		this->find_basic_minion()->setDEF(t_D);
		this->find_basic_minion()->setActCost(act_cost);
		
		return result;
	}
	if (this->getATK()!=0 || this->getDEF()!=0){
		return b->draw_enchantment_a_d(this);
	}
	else{
		return b->draw_enchantment(this);
	}

}

void draw_self_helper(Board *b, Minion *prev, vector<card_template_t> &ctt){
	Minion *next =prev->detach();
	// check if prev is a Basic Minion
	if (next == nullptr) return;
	
	draw_self_helper(b, next, ctt);

	// now know that prev is an Enchantment
	if (dynamic_cast<Enchantment*>(prev)->getAtkRep()!="" || dynamic_cast<Enchantment*>(prev) ->getDefRep()!=""){
		ctt.push_back(b->draw_enchantment_a_d(dynamic_cast<Enchantment*>(prev)));
	}
	else{
		ctt.push_back(b->draw_enchantment(dynamic_cast<Enchantment*>(prev)));
	}
}

vector<card_template_t> Enchantment::draw_self(Board *b){
	vector<card_template_t> ctt;
	if (this->parentMinion==nullptr){
		return ctt;
	}
	draw_self_helper(b, this, ctt);
	return ctt;
}

Minion* Enchantment::find_basic_minion(){
	if (this->parentMinion==nullptr){
		return nullptr;
	}
	Enchantment *e=this;
	Minion *prev= e;
	while (1){
		Minion *next =prev->detach();
		if (next == nullptr){
			return prev;
		}
		prev = next;
	}
	return nullptr;
}

Enchantment::~Enchantment(){
	//delete trigAbility
	if (trigAbility){
		vector<TriggeredAbility*> *trigs = getTrigAbilities();
		if (trigs){
			for (auto it = trigs->begin(); it != trigs->end(); ++it){
				if (*it == trigAbility){
					trigs->erase(it);
					delete trigAbility;
					break;
				}
			}
		}else{
			delete trigAbility;
		}
	}

	delete actAbility;
}
