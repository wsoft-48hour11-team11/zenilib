#include "AnimationObject.h"

using namespace std;
using namespace Zeni;

const float ANIMATION_FRAME_FREQUENCY = 0.1f;

AnimationObject::AnimationObject(const Zeni::Point2f &pos)
  : Object(pos)
{
	m_animation_index = 0;
	m_animation_frequency = 0.1f;
	m_frame_time = 0;
	setDeleteThis(false);
}

AnimationObject::~AnimationObject()
{
}

bool AnimationObject::getDeleteThis()
{
	return m_delete;
}

void AnimationObject::setDeleteThis(bool deleteThis)
{
	m_delete = deleteThis;
}

void AnimationObject::step(const float &time_step)
{
	Object::step(time_step);
	m_frame_time += time_step;
	if (m_frame_time > m_animation_frequency)
	{
		++m_animation_index;
		m_frame_time -= m_animation_frequency;
	}

}

void AnimationObject::render(const Zeni::Vector2f &offset)
{
	if (m_animation_index < int(m_animation_frames.size()))
	{
		Object::render(offset, m_animation_frames[m_animation_index], false);
	}
}
