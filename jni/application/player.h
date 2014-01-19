#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "BloodAnimation.h"

#include "common.h"

class Player : public Object {
public:
  enum State {STATE_IN_AIR,
              STATE_ON_WALL,
              STATE_ON_GROUND,
              STATE_ON_LOWER_LEFT,
              STATE_ON_LOWER_RIGHT
             };

  Player();
  Player(const Zeni::Point2f &position_);

  std::vector<Power> get_powers() const;
  bool has_power(const Power &power) const;
  void add_power(const Power &power);
  void remove_power(const Power &power);

  void killPlayer();
  bool isDead();

  void step(const float &time_step);

  void render(const Zeni::Vector2f &offset);

  std::pair<Zeni::Point2f, Zeni::Point2f> collision_box() const;
  
  bool jump;
  State state;

  bool left;
  bool right;
  float left_right;
  bool moving_right;

private:
  Zeni::Vector2f m_speed;
  std::vector<bool> m_powers;
  bool m_alive;
  BloodAnimation m_blood_animation;
};


#endif
