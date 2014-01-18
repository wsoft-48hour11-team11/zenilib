#ifndef OBJECT_H
#define OBJECT_H

#include <zenilib.h>

#include "common.h"

class Object {
public:
  Object() {}

  Object(const Zeni::Point2f &position_)
  : m_position(position_)
  {
  }

  virtual ~Object() {}
  
  const Zeni::Point2f & get_position() const {return m_position;}
  const Zeni::Vector2f & get_velocity() const {return m_velocity;}

  void set_velocity(const Zeni::Vector2f &velocity) {m_velocity = velocity;}

  virtual void step(const float &time_step) {
    m_velocity += time_step * m_acceleration;
    m_position += time_step * m_velocity;
  }

  virtual std::pair<Zeni::Point2f, Zeni::Point2f> collision_box() const {
    return std::make_pair(get_position(), get_position() + Zeni::Vector2f(1.0f, 1.0f));
  }

  void render(const Zeni::Vector2f &offset, const Zeni::String &texture, const bool &horizonally_flipped) {
    Zeni::render_image(texture, offset + TILE_SIZE * m_position, offset + TILE_SIZE * m_position + Zeni::Vector2f(TILE_SIZE, TILE_SIZE), horizonally_flipped);
  }

  virtual void render(const Zeni::Vector2f &offset) = 0;
 
private:
  Zeni::Point2f m_position;

  Zeni::Vector2f m_velocity;
  Zeni::Vector2f m_acceleration;
};

#endif
