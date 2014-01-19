#ifndef BAMFCLOUD_H
#define BAMFCLOUD_H

#include <zenilib.h>

#include "object.h"

class BamfCloud : public Object
{
	public:
		BamfCloud(const Zeni::Point2f &pos);
		~BamfCloud();

		bool getDeleteThis();
		void setDeleteThis(bool deleteThis);

		virtual void step(const float &time_step);
		void render(const Zeni::Vector2f &offset);

	protected:
		bool m_delete;

	private:
		std::vector<Zeni::String> m_animation_frames;
		int m_animation_index;
		float m_frame_time;

};


#endif
