#ifndef ENEMY_H
#define ENEMY_H

#include <zenilib.h>

#include "object.h"

class Enemy : public Object {
	public:
		Enemy(const Zeni::Point2f &pos);
		~Enemy();
};


#endif
