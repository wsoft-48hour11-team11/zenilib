#ifndef PORTAL_H
#define PORTAL_H

#include <zenilib.h>

#include "object.h"

class Portal : public Object {
	public:
    Portal::Portal(const Zeni::Point2f &pos)
      : Object(pos)
    {
    }

    Portal::~Portal()
    {
    }

		void step(const float &/*time_step*/) {
    }

		void render(const Zeni::Vector2f &offset) {
      Object::render(offset, "portal", false);
    }
};


#endif
