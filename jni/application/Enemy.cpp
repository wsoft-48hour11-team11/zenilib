#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

Zeni::Point2f Enemy::getPos()
{
	return m_pos;
}

void Enemy::setPos(Zeni::Point2f pos)
{
	m_pos = pos;
}
