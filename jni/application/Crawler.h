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
		Crawler(Zeni::Point2f pos, STATE state);
		~Crawler();

		virtual void update(float time);
		virtual void render();

	private:
		STATE m_state;

};

#endif
