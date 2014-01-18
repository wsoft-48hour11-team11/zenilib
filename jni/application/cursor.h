#ifndef CURSOR_H
#define CURSOR_H

#include <zenilib.h>

class Cursor
{
	public:
		Cursor();
		~Cursor();

		Zeni::Point2f getPos();
		void setPos(Zeni::Point2f pos);

		Zeni::Point2f getDimensions();
		void setDimensions(float width, float height);

		float getThickness();
		void setThickness(float thickness);

		Zeni::String getColor();
		void setColor(Zeni::String color);

		void render();

	private:
		Zeni::Point2f m_pos;
		float m_width;
		float m_height;
		float m_thickness;
		Zeni::String m_color;

};

#endif
