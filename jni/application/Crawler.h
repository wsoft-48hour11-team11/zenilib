#ifndef CRAWLER_H
#define CRAWLER_H

#include "Enemy.h"

class Crawler : public Enemy
{
	public:
		enum STATE
		{
			MOVING_LEFT,
			MOVING_RIGHT
		};

		Crawler();
		Crawler(const Zeni::Point2f &pos, const STATE &state);
		~Crawler();

		virtual void applyCollisionEffect(Player& player);

    void switch_direction() {m_state = m_state == MOVING_LEFT ? MOVING_RIGHT : MOVING_LEFT;}

		virtual void step(const float &time_step);
		void render(const Zeni::Vector2f &offset);

	private:
		STATE m_state;

};

#endif
