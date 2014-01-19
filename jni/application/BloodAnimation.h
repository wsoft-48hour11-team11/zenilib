#ifndef BLOODANIMATION_H
#define BLOODANIMATION_H

#include <zenilib.h>

#include "AnimationObject.h"

class BloodAnimation : public AnimationObject
{
	public:
		BloodAnimation(const Zeni::Point2f &pos);
		~BloodAnimation();
};


#endif
