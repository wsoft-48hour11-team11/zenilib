#ifndef POWERSEAL_H
#define POWERSEAL_H

#include "common.h"
#include "object.h"

class PowerSeal : public Object
{
	public:		
		PowerSeal(const Zeni::Point2f &position, const Power &power);
		~PowerSeal();

		Power getPower();
		void setPower(Power power);

    void render(const Zeni::Vector2f &offset);

	private:
		Power m_power;
};

#endif
