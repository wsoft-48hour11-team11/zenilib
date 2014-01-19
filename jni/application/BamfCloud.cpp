#include "BamfCloud.h"

using namespace std;
using namespace Zeni;

BamfCloud::BamfCloud(const Zeni::Point2f &pos)
  : AnimationObject(pos)
{
	m_animation_frames.push_back("bamf1");
	m_animation_frames.push_back("bamf2");
	m_animation_frames.push_back("bamf3");
	m_animation_frames.push_back("bamf4");
}

BamfCloud::~BamfCloud()
{
}
