#include "Play_State.h"

using namespace std;
using namespace Zeni;

Play_State::Play_State()
	: m_grid(Zeni::Point2i(50, 32), Point2i(0, 0))
{
	set_pausable(true);

	//m_grid.load("test_level.txt");
	GameSingleton* sing = GameSingleton::getInstance();
	m_grid.load(sing->level_list[0]);

	m_player = Player(Zeni::Point2f(m_grid.get_spawn_player().x + 4.5f / 16.0f, float(m_grid.get_spawn_player().y)), Zeni::Vector2f(7.0f / 16.0f, 1.0f));
}

Play_State::~Play_State() {
	//m_grid.save("test_level.txt");
}

void Play_State::on_push() {
	get_Window().set_mouse_state(Window::MOUSE_HIDDEN);
}

void Play_State::on_pop() {
	get_Controllers().reset_vibration_all();
}

void Play_State::on_cover() {
get_Controllers().reset_vibration_all();
}

void Play_State::on_controller_button(const SDL_ControllerButtonEvent &event) {
	if(event.button == SDL_CONTROLLER_BUTTON_BACK && event.state == SDL_PRESSED)
	    get_Game().push_Popup_Menu_State();
}

void Play_State::prerender() {
	get_Video().set_clear_Color(Color());
}

void Play_State::render() {
	Video &vr = get_Video();

	vr.set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(float(m_grid.get_width()), float(m_grid.get_height()))), true);

	m_grid.render();
	m_player.render();
}