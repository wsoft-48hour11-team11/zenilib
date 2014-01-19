#ifndef BAMFCLOUD_H
#define BAMFCLOUD_H

#include <zenilib.h>

#include "AnimationObject.h"

class BamfCloud : public AnimationObject
{
	public:
		BamfCloud(const Zeni::Point2f &pos);
		~BamfCloud();
};


#endif
