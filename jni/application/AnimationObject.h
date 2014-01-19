#ifndef ANIMATIONOBJECT_H
#define ANIMATIONOBJECT_H

#include <zenilib.h>

#include "object.h"

class AnimationObject : public Object
{
	public:
		AnimationObject(const Zeni::Point2f &pos);
		~AnimationObject();

		bool getDeleteThis();
		void setDeleteThis(bool deleteThis);

		void step(const float &time_step);
		void render(const Zeni::Vector2f &offset);

	protected:
		bool m_delete;
		std::vector<Zeni::String> m_animation_frames;
		int m_animation_index;
		float m_animation_frequency;

	private:
		float m_frame_time;

};

#endif
