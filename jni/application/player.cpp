#include "player.h"

Player::Player()
: Object(Zeni::Point2f()),
jump(false),
state(STATE_IN_AIR),
left(false),
right(false),
left_right(0.0f),
moving_right(true),
m_speed(8.0f, 8.0f),
m_powers(POWER_LIST_SIZE, true)
{
  m_powers[POWER_EMPTY] = false;
}

Player::Player(const Zeni::Point2f &position_)
: Object(position_),
jump(false),
state(STATE_IN_AIR),
left(false),
right(false),
left_right(0.0f),
moving_right(true),
m_speed(8.0f, 8.0f),
m_powers(POWER_LIST_SIZE, true)
{
  m_powers[POWER_EMPTY] = false;
}

std::vector<Power> Player::get_powers() const {
  std::vector<Power> powers;
  for(int i = 1; i != POWER_LIST_SIZE; ++i) {
    if(m_powers[i])
      powers.push_back(Power(i));
  }
  return powers;
}

bool Player::has_power(const Power &power) const {
  return m_powers[power];
}

void Player::add_power(const Power &power) {
  assert(!m_powers[power]);
  m_powers[power] = true;
if (power == POWER_SPEED)
{
	m_speed *= 4;
}
}

void Player::remove_power(const Power &power) {
  assert(m_powers[power]);
  m_powers[power] = false;
if (power == POWER_SPEED)
{
	m_speed *= 0.25;
}
}

void Player::step(const float &time_step) {
  if(jump && state != STATE_IN_AIR) {
    set_velocity(Zeni::Vector2f(get_velocity().i, -8.0f));
    jump = false;
    state = STATE_IN_AIR;
  }

  switch(state) {
  case STATE_IN_AIR:
  case STATE_ON_GROUND:
  case STATE_ON_WALL:
    set_velocity(Zeni::Vector2f(0.25f * left_right * m_speed.i + 0.75f * get_velocity().i, get_velocity().j));
    break;

  case STATE_ON_LOWER_LEFT:
    if(left_right > 0.0f)
      set_velocity(0.5f * Zeni::Vector2f(1.0f, 1.75f).normalized() * m_speed.i + 0.5f * get_velocity());
    else if(left_right < 0.0f)
      set_velocity(0.5f * Zeni::Vector2f(-1.0f, -0.25f).normalized() * m_speed.i + 0.5f * get_velocity());
    else
      set_velocity(0.5f * get_velocity());
    break;

  case STATE_ON_LOWER_RIGHT:
    if(left_right > 0.0f)
      set_velocity(0.5f * Zeni::Vector2f(1.0f, -0.25f).normalized() * m_speed.i + 0.5f * get_velocity());
    else if(left_right < 0.0f)
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

void Player::render(const Zeni::Vector2f &offset) {
  Object::render(offset, "player", false);
}

std::pair<Zeni::Point2f, Zeni::Point2f> Player::collision_box() const {
  return std::make_pair(get_position() + Zeni::Vector2f(0.0f, 2.0f / 16.0f), get_position() + Zeni::Vector2f(7.0f / 16.0f, 1.0f));
}
