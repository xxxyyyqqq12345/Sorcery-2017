#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include <string>
#include "board_place.h"

class Player;
class Observer {
 public:
  virtual void notify(BoardPlace bp, Player *p) = 0;
  virtual void notify(Player *AP, Player *NAP) = 0;
  virtual ~Observer() = default;
};
#endif
