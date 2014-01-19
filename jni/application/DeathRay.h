#ifndef DEATHRAY_H
#define DEATHRAY_H

#include <zenilib.h>

#include "Enemy.h"
#include "object.h"

class DeathRay : public Object {
	public:
		enum STATE
		{
			MOVING_LEFT,
			MOVING_RIGHT
		};

		DeathRay(const Zeni::Point2f &pos, const STATE &state);
		~DeathRay();

		std::pair<Zeni::Point2f, Zeni::Point2f> getCollisionBox();

		void applyCollisionEffect(Enemy& enemy);

		bool getDeleteThis();
		void setDeleteThis(bool deleteThis);

		virtual void step(const float &time_step);
		void render(const Zeni::Vector2f &offset);

	protected:
		void setCollisionBox(Zeni::Point2f upper_left, Zeni::Point2f lower_right);
		Zeni::Point2f m_collision_ul;
		Zeni::Point2f m_collision_lr;
		bool m_delete;

	private:
		STATE m_state;
};


#endif
