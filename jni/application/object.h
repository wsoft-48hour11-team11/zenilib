#ifndef OBJECT_H
#define OBJECT_H

#include <zenilib.h>

class Object {
public:
  Object() {}

  Object(const Zeni::Point2f &position_,
         const Zeni::Vector2f &size_)
  : m_position(position_),
    m_size(size_)
  {
  }

  virtual ~Object() {}
  
  const Zeni::Point2f & get_position() const {return m_position;}
  const Zeni::Vector2f & get_size() const {return m_size;}

  void step(const float &time_step) {
    m_velocity += time_step * m_acceleration;
    m_position += time_step * m_velocity;
  }

  void render(const Zeni::String &texture, const bool &horizonally_flipped) {
    Zeni::render_image(texture, m_position, m_position + m_size, horizonally_flipped);
  }
 
private:
  Zeni::Point2f m_position;
  Zeni::Vector2f m_size;

  Zeni::Vector2f m_velocity;
  Zeni::Vector2f m_acceleration;
};

#endif
