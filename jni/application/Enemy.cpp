#include "Enemy.h"

using namespace std;

Enemy::Enemy(const Zeni::Point2f &pos)
  : Object(pos)
{
}

Enemy::~Enemy()
{
}

std::pair<Zeni::Point2f, Zeni::Point2f> Enemy::getCollisionBox()
{
	return make_pair(m_collision_ul, m_collision_lr);
}

void Enemy::setCollisionBox(Zeni::Point2f upper_left, Zeni::Point2f lower_right)
{
	m_collision_ul = upper_left;
	m_collision_lr = lower_right;
}
