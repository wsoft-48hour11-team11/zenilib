#include "LevelEditor.h"

using namespace Zeni;
using namespace std;

LevelEditor::LevelEditor()
{
	m_grid_cursor_pos = Point2i(0, 0);
	m_cursor.setDimensions(16, 16);
	m_cursor.setThickness(4);
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::on_cover()
{
}

void LevelEditor::on_uncover()
{
}

void LevelEditor::on_push()
{
}

void LevelEditor::on_pop()	
{
}

void LevelEditor::on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action)	
{
}

void LevelEditor::perform_logic()
{
}

void LevelEditor::step(const float &time_step)
{
}

void LevelEditor::render()
{
	Video &vr = get_Video();
	vr.set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(float(m_level.get_width()) * 16.0f, float(m_level.get_height()) * 16.0f)), true);

	//m_level.render();
	m_cursor.setPos(Point2f(m_grid_cursor_pos.x * 16.0f, m_grid_cursor_pos.y * 16.0f));
	m_cursor.render();
}
