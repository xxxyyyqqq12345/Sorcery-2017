#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream> 
#include <string>
#include "board_place.h"
#include "observer.h"
#include "ascii_graphics.h"

class BasicMinion;
class Minion;
class Ritual;
class Spell;
class Enchantment;
class Player;
class Xwindow;
class Board: public Observer{
  // graphics values
  const int start = 34;

  // Area values
  const int aRitual = 0, aPlayer = 1, aGrave = 2;
  //AP
  const int APMinX[5] {34, 267, 500, 733, 966};
  const int APMinY = 388;
  const int APAreaX[3] {34, 500, 966};
  const int APAreaY = 547;
  const int APHandX[5] {80, 290, 500, 710, 920};
  const int APHandY = 706;

  //NAP
  const int NAPMinX[5] {34, 267, 500, 733, 966};
  const int NAPMinY = 164;
  const int NAPAreaX[3] {34, 500, 966};
  const int NAPAreaY = 5;

  Xwindow *xw = nullptr;
  void draw_minion_graphics(Player *p);
  void draw_ritual_graphics(Player *p);
  void draw_grave_graphics(Player *p);
  void draw_player_graphics(Player *p);
 public:
  Board() = default;
  void attach_graphics(Xwindow *xw);
  void display_hand_graphics(Player *p);
  void display_board_graphics(Player *p1,Player *p2);
  virtual void display_board(Player *p1,Player *p2);
  virtual void display_hand(Player *p);
  virtual void display_minion(Minion *m);
  virtual card_template_t draw_minion_ne(BasicMinion *m);
  virtual card_template_t draw_minion_ae(BasicMinion *m);
  virtual card_template_t draw_minion_te(BasicMinion *m);
  virtual card_template_t draw_ritual(Ritual *r);
  virtual card_template_t draw_spell(Spell *s);
  virtual card_template_t draw_enchantment(Enchantment *e);
  virtual card_template_t draw_enchantment_a_d(Enchantment *e);
  void notify(Player *p1,Player *p2);
  void notify(BoardPlace bp, Player *p);
};


#endif
