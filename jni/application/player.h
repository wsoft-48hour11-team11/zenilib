#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"

#include "common.h"

class Player : public Object {
public:
  enum State {STATE_IN_AIR,
              STATE_ON_WALL,
              STATE_ON_GROUND,
              STATE_ON_LOWER_LEFT,
              STATE_ON_LOWER_RIGHT
             };

  Player()
  : Object(Zeni::Point2f()),
  jump(false),
  state(STATE_IN_AIR),
  left(false),
  right(false),
  m_speed(8.0f, 8.0f),
  m_powers(POWER_LIST_SIZE, true)
  {
    m_powers[POWER_EMPTY] = false;
  }

  Player(const Zeni::Point2f &position_)
  : Object(position_),
  jump(false),
  state(STATE_IN_AIR),
  left(false),
  right(false),
  m_speed(8.0f, 8.0f),
  m_powers(POWER_LIST_SIZE, true)
  {
    m_powers[POWER_EMPTY] = false;
  }

  std::vector<Power> get_powers() const {
    std::vector<Power> powers;
    for(int i = 1; i != POWER_LIST_SIZE; ++i) {
      if(m_powers[i])
        powers.push_back(Power(i));
    }
    return powers;
  }

  bool has_power(const Power &power) const {
    return m_powers[power];
  }

  void add_power(const Power &power) {
    assert(!m_powers[power]);
    m_powers[power] = true;
  }

  void remove_power(const Power &power) {
    assert(m_powers[power]);
    m_powers[power] = false;
  }

  void step(const float &time_step) {
    if(jump && state != STATE_IN_AIR) {
      set_velocity(Zeni::Vector2f(get_velocity().i, -8.0f));
      jump = false;
      state = STATE_IN_AIR;
    }

    switch(state) {
    case STATE_IN_AIR:
    case STATE_ON_GROUND:
      set_velocity(Zeni::Vector2f(0.25f * (right - left) * m_speed.i + 0.75f * get_velocity().i, get_velocity().j));
      break;

    case STATE_ON_LOWER_LEFT:
      if(right > left)
        set_velocity(0.5f * Zeni::Vector2f(1.0f, 1.75f).normalized() * m_speed.i + 0.5f * get_velocity());
      else if(left > right)
        set_velocity(0.5f * Zeni::Vector2f(-1.0f, -0.25f).normalized() * m_speed.i + 0.5f * get_velocity());
      else
        set_velocity(0.5f * get_velocity());
      break;

    case STATE_ON_LOWER_RIGHT:
      if(right > left)
        set_velocity(0.5f * Zeni::Vector2f(1.0f, -0.25f).normalized() * m_speed.i + 0.5f * get_velocity());
      else if(left > right)
        set_velocity(0.5f * Zeni::Vector2f(-1.0f, 1.75f).normalized() * m_speed.i + 0.5f * get_velocity());
      else
        set_velocity(0.5f * get_velocity());
      break;

    default:
      break;
    }

    Object::step(time_step);

    state = STATE_IN_AIR;
  }

  void render(const Zeni::Vector2f &offset) {
    Object::render(offset, "player", false);
  }

  std::pair<Zeni::Point2f, Zeni::Point2f> collision_box() const {
    return std::make_pair(get_position() + Zeni::Vector2f(0.0f, 2.0f / 16.0f), get_position() + Zeni::Vector2f(7.0f / 16.0f, 1.0f));
  }
  
  bool jump;
  State state;

  bool left;
  bool right;

private:
  Zeni::Vector2f m_speed;
  std::vector<bool> m_powers;
};


#endif
