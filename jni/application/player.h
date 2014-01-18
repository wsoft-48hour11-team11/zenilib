#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"

class Player : public Object {
public:
  Player() {}

  Player(const Zeni::Point2f &position_)
  : Object(position_),
  jump(false),
  can_jump(false),
  left(false),
  right(false),
  m_speed(8.0f, 8.0f)
  {
  }

  void step(const float &time_step) {
    set_velocity(Zeni::Vector2f((right - left) * m_speed.i, get_velocity().j));
    Object::step(time_step);
  }

  void render(const Zeni::Vector2f &offset) {
    Object::render(offset, "player", false);
  }

  std::pair<Zeni::Point2f, Zeni::Point2f> collision_box() const {
    return std::make_pair(get_position() + Zeni::Vector2f(2.0f, 0.0f), get_position() + Zeni::Vector2f(16.0f, 7.0f));
  }
  
  bool jump;
  bool can_jump;

  bool left;
  bool right;

private:
  Zeni::Vector2f m_speed;
};


#endif
