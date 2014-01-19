#include "BloodAnimation.h"

using namespace std;
using namespace Zeni;

BloodAnimation::BloodAnimation(const Zeni::Point2f &pos)
  : AnimationObject(pos)
{
	m_animation_frames.push_back("blood1");
	m_animation_frames.push_back("blood2");
	m_animation_frames.push_back("blood3");
	m_animation_frames.push_back("blood4");
	m_animation_frames.push_back("blood5");
}

BloodAnimation::~BloodAnimation()
{
}
