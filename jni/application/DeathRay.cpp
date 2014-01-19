#include "DeathRay.h"

using namespace std;
using namespace Zeni;

const float DEATHRAY_MOVEMENT_SPEED = 40.0f;

DeathRay::DeathRay(const Zeni::Point2f &pos, const DeathRay::STATE &state)
  : Object(pos),
  m_state(state)
{
	setDeleteThis(false);
}

DeathRay::~DeathRay()
{
}

std::pair<Zeni::Point2f, Zeni::Point2f> DeathRay::getCollisionBox()
{
	return make_pair(m_collision_ul, m_collision_lr);
}

void DeathRay::setCollisionBox(Zeni::Point2f upper_left, Zeni::Point2f lower_right)
{
	m_collision_ul = upper_left;
	m_collision_lr = lower_right;
}

void DeathRay::applyCollisionEffect(Enemy& enemy)
{
	enemy.setDeleteThis(true);
	setDeleteThis(true);
}

bool DeathRay::getDeleteThis()
{
	return m_delete;
}

void DeathRay::setDeleteThis(bool deleteThis)
{
	m_delete = deleteThis;
}

void DeathRay::step(const float &time_step)
{
	if (m_state == MOVING_LEFT)
		set_velocity(Vector2f(-DEATHRAY_MOVEMENT_SPEED, 0.0f));
	else if (m_state == MOVING_RIGHT)
		set_velocity(Vector2f(DEATHRAY_MOVEMENT_SPEED, 0.0f));

	Object::step(time_step);

	Point2f pos = get_position();
	setCollisionBox(pos, Point2f(pos.x + 1.0f, pos.y + 1.0f));
}

void DeathRay::render(const Zeni::Vector2f &offset)
{
	Object::render(offset, "deathray", m_state == MOVING_RIGHT);
}
