#include "cursor.h"

using namespace Zeni;

Cursor::Cursor()
{
	m_color = "green";
}

Cursor::~Cursor()
{
}

Zeni::Point2f Cursor::getPos()
{
	return m_pos;
}

void Cursor::setPos(Zeni::Point2f pos)
{
	m_pos = pos;
}

Zeni::Point2f Cursor::getDimensions()
{
	return Point2f(m_width, m_height);
}

void Cursor::setDimensions(float width, float height)
{
	m_width = width;
	m_height = height;
}

float Cursor::getThickness()
{
	return m_thickness;
}

void Cursor::setThickness(float thickness)
{
	m_thickness = thickness;
}

Zeni::String Cursor::getColor()
{
	return m_color;
}

void Cursor::setColor(Zeni::String color)
{
	m_color = color;
}

void Cursor::render()
{
	Video &vr = get_Video();
	Colors &cr = get_Colors();

	Vertex2f_Color p0(Point2f(m_pos.x, m_pos.y), cr[m_color]);
	Vertex2f_Color p1(Point2f(m_pos.x, m_pos.y + m_thickness), cr[m_color]);
	Vertex2f_Color p2(Point2f(m_pos.x + m_width, m_pos.y + m_thickness), cr[m_color]);
	Vertex2f_Color p3(Point2f(m_pos.x + m_width, m_pos.y), cr[m_color]);
	Quadrilateral<Vertex2f_Color> top(p0, p1, p2, p3);

	p0 = Vertex2f_Color(Point2f(m_pos.x, m_pos.y + m_height - m_thickness), cr[m_color]);
	p1 = Vertex2f_Color(Point2f(m_pos.x, m_pos.y + m_height), cr[m_color]);
	p2 = Vertex2f_Color(Point2f(m_pos.x + m_width, m_pos.y + m_height), cr[m_color]);
	p3 = Vertex2f_Color(Point2f(m_pos.x + m_width, m_pos.y + m_height - m_thickness), cr[m_color]);
	Quadrilateral<Vertex2f_Color> bottom(p0, p1, p2, p3);

	p0 = Vertex2f_Color(Point2f(m_pos.x, m_pos.y), cr[m_color]);
	p1 = Vertex2f_Color(Point2f(m_pos.x, m_pos.y + m_height), cr[m_color]);
	p2 = Vertex2f_Color(Point2f(m_pos.x + m_thickness, m_pos.y + m_height), cr[m_color]);
	p3 = Vertex2f_Color(Point2f(m_pos.x + m_thickness, m_pos.y), cr[m_color]);
	Quadrilateral<Vertex2f_Color> left(p0, p1, p2, p3);

	p0 = Vertex2f_Color(Point2f(m_pos.x + m_width - m_thickness, m_pos.y), cr[m_color]);
	p1 = Vertex2f_Color(Point2f(m_pos.x + m_width - m_thickness, m_pos.y + m_height), cr[m_color]);
	p2 = Vertex2f_Color(Point2f(m_pos.x + m_width, m_pos.y + m_height - m_thickness), cr[m_color]);
	p3 = Vertex2f_Color(Point2f(m_pos.x + m_width, m_pos.y), cr[m_color]);
	Quadrilateral<Vertex2f_Color> right(p0, p1, p2, p3);

	vr.render(top);
	vr.render(bottom);
	vr.render(left);
	vr.render(right);
}
