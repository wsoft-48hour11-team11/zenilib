#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"

class Player : public Object {
public:
  Player() {}

  Player(const Zeni::Point2f &position_,
         const Zeni::Vector2f &size_)
  : Object(position_, size_)
  {
  }

  void render() {
    Object::render("player", false);
  }
};


#endif
