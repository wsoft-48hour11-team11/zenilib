#ifndef ENEMY_H
#define ENEMY_H

#include <zenilib.h>

#include "player.h"
#include "object.h"

class Enemy : public Object {
	public:
		Enemy(const Zeni::Point2f &pos);
		~Enemy();

		//Returns the offsets of the box from getPos
		std::pair<Zeni::Point2f, Zeni::Point2f> getCollisionBox();

		virtual void applyCollisionEffect(Player& player) = 0;

		bool getDeleteThis();
		void setDeleteThis(bool deleteThis);

	protected:
		void setCollisionBox(Zeni::Point2f upper_left, Zeni::Point2f lower_right);
		Zeni::Point2f m_collision_ul;
		Zeni::Point2f m_collision_lr;
		bool m_delete;
};


#endif
