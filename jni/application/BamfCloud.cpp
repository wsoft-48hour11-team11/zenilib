#include "BamfCloud.h"

using namespace std;
using namespace Zeni;

const float ANIMATION_FRAME_FREQUENCY = 0.1f;

BamfCloud::BamfCloud(const Zeni::Point2f &pos)
  : Object(pos)
{
	m_animation_frames.push_back("bamf1");
	m_animation_frames.push_back("bamf2");
	m_animation_frames.push_back("bamf3");
	m_animation_frames.push_back("bamf4");
	m_animation_index = 0;
	setDeleteThis(false);
}

BamfCloud::~BamfCloud()
{
}

bool BamfCloud::getDeleteThis()
{
	return m_delete;
}

void BamfCloud::setDeleteThis(bool deleteThis)
{
	m_delete = deleteThis;
}

void BamfCloud::step(const float &time_step)
{
	

	Object::step(time_step);
	m_frame_time += time_step;
	if (m_frame_time > ANIMATION_FRAME_FREQUENCY)
	{
		++m_animation_index;
		m_frame_time -= ANIMATION_FRAME_FREQUENCY;
	}

}

void BamfCloud::render(const Zeni::Vector2f &offset)
{
	if (m_animation_index < m_animation_frames.size())
	{
		Object::render(offset, m_animation_frames[m_animation_index], false);
	}
}
