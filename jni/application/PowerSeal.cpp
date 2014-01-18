#include "PowerSeal.h"

using namespace Zeni;

PowerSeal::PowerSeal(const Zeni::Point2f &position, const Power &power)
  : Object(position),
  m_power(power)
{
}

PowerSeal::~PowerSeal()
{
}

Power PowerSeal::getPower()
{
	return m_power;
}

void PowerSeal::setPower(Power power)
{
	m_power = power;
}

void PowerSeal::render(const Zeni::Vector2f &offset)
{
  Object::render(offset, power_asset(m_power), false);
}
