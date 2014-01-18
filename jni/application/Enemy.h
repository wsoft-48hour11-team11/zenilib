#ifndef ENEMY_H
#define ENEMY_H

#include <zenilib.h>


class Enemy
{
	public:
		Enemy();
		~Enemy();

		virtual void update(float time) = 0;
		virtual void render() = 0;

		virtual Zeni::Point2f getPos();
		virtual void setPos(Zeni::Point2f pos);

	protected:
		Zeni::Point2f m_pos;

};


#endif
