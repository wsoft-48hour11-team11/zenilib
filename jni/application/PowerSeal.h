#ifndef POWERSEAL_H
#define POWERSEAL_H

#include "common.h"

class PowerSeal
{
	public:		
		PowerSeal();
		~PowerSeal();

		Zeni::Point2f getUpperLeft();
		void setUpperLeft(Zeni::Point2f upper_left);

		Zeni::Point2f getLowerRight();
		void setLowerRight(Zeni::Point2f lower_right);

		Power getPower();
		void setPower(Power power);

		void render();

	private:
		Power m_power;
		Zeni::Point2f m_upper_left;
		Zeni::Point2f m_lower_right;
};

#endif
