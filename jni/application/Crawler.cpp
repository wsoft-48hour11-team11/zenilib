#include "Crawler.h"

#include <zenilib.h>

using namespace Zeni;

const float MOVEMENT_SPEED = 10;

Crawler::Crawler()
{
	m_state = MOVING_LEFT;
}

Crawler::Crawler(Zeni::Point2f pos, Crawler::STATE state)
{
	setPos(pos);
	m_state = state;
}

Crawler::~Crawler()
{
}

void Crawler::update(float time)
{
	if (m_state == MOVING_LEFT)
	{
		m_pos.x -= MOVEMENT_SPEED * time;
	}
	else if (m_state == MOVING_RIGHT)
	{
		m_pos.x += MOVEMENT_SPEED * time;
	}

}

void Crawler::render()
{
	if (m_state == MOVING_LEFT)
	{
		Zeni::render_image("player", m_pos, Point2f(m_pos.x + 16, m_pos.y + 16), false);
	}
	else //if (m_state == MOVING_RIGHT)
	{
		Zeni::render_image("player", m_pos, Point2f(m_pos.x + 16, m_pos.y + 16), true);
	}
}
