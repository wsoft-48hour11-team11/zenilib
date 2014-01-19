#include "PowerSelect.h"

using namespace std;
using namespace Zeni;

PowerSelect::PowerSelect(Zeni::Gamestate gamestate, Player* player, PowerSeal* powerseal)
	: m_game_state(gamestate),
	tb(Point2f(0, 140), Point2f(RES_HORIZ, 200),"intro", "Select the power to seal:", get_Colors()["white"]),
	tb2(Point2f(RES_HORIZ - 128.0f, RES_VERT - 32.0f), Point2f(RES_HORIZ, RES_VERT),"intro", "Press Enter", get_Colors()["white"])

{
	//Set up text boxes
	Colors &cr = get_Colors();
	tb.give_BG_Renderer(new Widget_Renderer_Color(cr["red"]));
	tb2.give_BG_Renderer(new Widget_Renderer_Color(cr["black"]));
	
	//Set up the rest
	m_player = player;
	m_powerseal = powerseal;

	m_cursor_index = 0;
	m_max_cursor_index = m_player->get_powers().size() - 1;

	//Set up cursor
	m_cursor.setColor("blue");
	m_cursor.setPos(Point2f(544.0f, 200.0f));
	m_cursor.setDimensions(32.0f, 32.0f);
	m_cursor.setThickness(4.0f);

	//Setup actions
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_a), 1);	//DPAD_LEFT
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_d), 2);	//DPAD_RIGHT
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_LEFT), 1);	//DPAD_LEFT
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_RIGHT), 2);	//DPAD_RIGHT
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_RETURN), 3);	//ENTER
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_DELETE), 4);	//BACK
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_BACKSPACE), 4);	//BACK

  set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_DPAD_LEFT), 1);	//DPAD_LEFT
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_DPAD_RIGHT), 2);	//DPAD_RIGHT
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_START), 3);	//ENTER
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_A), 3);	//ENTER
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_BACK), 4);	//BACK
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_Y), 4);	//BACK
}

PowerSelect::~PowerSelect()
{
}

void PowerSelect::on_cover()
{
}

void PowerSelect::on_uncover()
{
}

void PowerSelect::on_push()
{
}

void PowerSelect::on_pop()
{
}

void PowerSelect::on_event(const Zeni::Zeni_Input_ID &/*id*/, const float &confidence, const int &action)
{
	if (action == 1)
	{
		//DPAD_LEFT
		if (confidence == 1.0)
		{
			if (m_cursor_index > 0)
			{
				--m_cursor_index;
			}
		}
	}
	if (action == 2)
	{
		//DPAD_RIGHT
		if (confidence == 1.0)
		{
			if (m_cursor_index < m_max_cursor_index)
			{
				++m_cursor_index;
			}
		}
	}
	if (action == 3)
	{
		//ENTER
		if (confidence == 1.0)
		{
			Power lost_power = m_player->get_powers()[m_cursor_index];
			m_powerseal->setPower(lost_power);
			m_player->remove_power(lost_power);
			get_Game().pop_state();
      Zeni::play_sound("chest");
		}
	}
	if (action == 4)
	{
		//BACK
		if (confidence == 1.0)
			get_Game().pop_state();
	}
}

void PowerSelect::perform_logic()
{
}

void PowerSelect::step(const float &/*time_step*/)
{
}

void PowerSelect::render()
{
	Video &vr = get_Video();
	Colors &cr = get_Colors();
	
	m_game_state.render();

	vr.set_2d(make_pair(Point2f(), Point2f(RES_HORIZ, RES_VERT)), true);

	//Render faded bg
	Vertex2f_Color p0(Point2f(0, 0), cr["transblack"]);
	Vertex2f_Color p1(Point2f(0, RES_VERT), cr["transblack"]);
	Vertex2f_Color p2(Point2f(RES_HORIZ, RES_VERT), cr["transblack"]);
	Vertex2f_Color p3(Point2f(RES_HORIZ, 0), cr["transblack"]);
	Quadrilateral<Vertex2f_Color> top(p0, p1, p2, p3);

	vr.render(top);

	//Render the available powers
	Point2f base_pos = Point2f(544.0f, 200.0f);
	for (int i = 0; i <= m_max_cursor_index; i++)
	{
		Point2f image_ul = Point2f(base_pos.x + i * 48, base_pos.y);
		Point2f image_lr = Point2f(image_ul.x + 32, image_ul.y + 32.0f);
		vector<Power> pow = m_player->get_powers();
		render_image(power_asset(m_player->get_powers()[i]), image_ul, image_lr);
	}

	m_cursor.setPos(Point2f(base_pos.x + m_cursor_index * 48, base_pos.y));
	m_cursor.render();

	//Render the power's name
	get_Fonts()["intro"].render_text("Select the power to seal:",
										Point2f(RES_HORIZ/2.0f, 140),
										cr["white"],
										ZENI_CENTER );
	get_Fonts()["intro"].render_text(power_name(m_player->get_powers()[m_cursor_index]),
										Point2f(RES_HORIZ/2.0f, 240),
										cr["white"],
										ZENI_CENTER );
}
