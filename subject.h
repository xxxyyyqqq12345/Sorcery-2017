#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include <vector>
#include "board_place.h"

class Observer;
class Player;
class Subject {
  std::vector<Observer*> observers;
 public:
  void attach(Observer *o);
  void notifyObservers(BoardPlace bp, Player *p);
  void notifyObservers(Player *AP, Player *NAP);
  virtual ~Subject() = default;
};

#endif
