#include "Crawler.h"

#include <zenilib.h>

using namespace Zeni;

const float MOVEMENT_SPEED = 10;

Crawler::Crawler()
  : Enemy(Point2f())
{
	m_state = MOVING_LEFT;
}

Crawler::Crawler(const Zeni::Point2f &pos, const Crawler::STATE &state)
  : Enemy(pos)
{
	m_state = state;
}

Crawler::~Crawler()
{
}

void Crawler::step(const float &time_step)
{
	if (m_state == MOVING_LEFT)
	{
    set_velocity(Vector2f(-MOVEMENT_SPEED * time_step, 0.0f));
	}
	else if (m_state == MOVING_RIGHT)
	{
    set_velocity(Vector2f(MOVEMENT_SPEED * time_step, 0.0f));
	}

}

void Crawler::render(const Zeni::Vector2f &offset)
{
  Object::render(offset, "player", m_state == MOVING_RIGHT);
}
