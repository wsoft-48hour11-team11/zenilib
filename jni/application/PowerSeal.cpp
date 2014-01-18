#include "PowerSeal.h"

using namespace Zeni;

PowerSeal::PowerSeal()
{
	m_power = POWER_EMPTY;
}

PowerSeal::~PowerSeal()
{
}

Zeni::Point2f PowerSeal::getUpperLeft()
{
	return m_upper_left;
}

void PowerSeal::setUpperLeft(Zeni::Point2f upper_left)
{
	m_upper_left = upper_left;
}

Zeni::Point2f PowerSeal::getLowerRight()
{
	return m_lower_right;
}

void PowerSeal::setLowerRight(Zeni::Point2f lower_right)
{
	m_lower_right = lower_right;
}

Power PowerSeal::getPower()
{
	return m_power;
}

void PowerSeal::setPower(Power power)
{
	m_power = power;
}

void PowerSeal::render()
{
	render_image(power_asset(m_power), m_upper_left, m_lower_right);
}
