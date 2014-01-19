#include "Crawler.h"

#include <zenilib.h>

using namespace std;
using namespace Zeni;

const float MOVEMENT_SPEED = 4;

Crawler::Crawler()
  : Enemy(Point2f())
{
	m_state = MOVING_LEFT;
	Point2f posi = get_position();
	setCollisionBox(posi, Point2f(posi.x + 1.0f, posi.y + 1.0f));
}

Crawler::Crawler(const Zeni::Point2f &pos, const Crawler::STATE &state)
  : Enemy(pos)
{
	m_state = state;
	Point2f posi = get_position();
	setCollisionBox(posi, Point2f(posi.x + 1.0f, posi.y + 1.0f));
}

Crawler::~Crawler()
{
}

void Crawler::applyCollisionEffect(Player &/*player*/)
{
}

void Crawler::step(const float &time_step)
{
	if (m_state == MOVING_LEFT)
		set_velocity(Vector2f(-MOVEMENT_SPEED, 0.0f));
	else if (m_state == MOVING_RIGHT)
		set_velocity(Vector2f(MOVEMENT_SPEED, 0.0f));

	Object::step(time_step);

	Point2f pos = get_position();
	setCollisionBox(pos, Point2f(pos.x + 1.0f, pos.y + 1.0f));

}

void Crawler::render(const Zeni::Vector2f &offset)
{
  Object::render(offset, "crawler1", m_state == MOVING_RIGHT);
}
