#ifndef ENEMY_H
#define ENEMY_H

#include <zenilib.h>

#include "player.h"
#include "object.h"

class Enemy : public Object {
	public:
		Enemy(const Zeni::Point2f &pos);
		~Enemy();

		std::pair<Zeni::Point2f, Zeni::Point2f> getCollisionBox();

		virtual void applyCollisionEffect(Player& player) = 0;

    virtual void switch_direction() = 0;

		bool getDeleteThis();
		void setDeleteThis(bool deleteThis);

    void play_sound() {
      if(!m_shadow_source.is_playing() || m_shadow_source.get_time() >= 2.0f) {
        m_shadow_source.set_time(0.0f);
        m_shadow_source.play();
      }
    }

	protected:
		void setCollisionBox(Zeni::Point2f upper_left, Zeni::Point2f lower_right);
		Zeni::Point2f m_collision_ul;
		Zeni::Point2f m_collision_lr;
		bool m_delete;

    Zeni::Sound_Source m_shadow_source;
};


#endif
