#include "VictoryState.h"

#include <fstream>

#include "common.h"
#include "GameSingleton.h"
#include "Play_State.h"

using namespace std;
using namespace Zeni;

VictoryState::VictoryState()
  : tb(Point2f(0, 0), Point2f(RES_HORIZ, RES_VERT),"intro", "", get_Colors()["white"]),
	tb2(Point2f(RES_HORIZ - 128.0f, RES_VERT - 32.0f), Point2f(RES_HORIZ, RES_VERT),"intro", "Press Enter", get_Colors()["white"])
{
	//Set up text boxes
	Colors &cr = get_Colors();
	tb.give_BG_Renderer(new Widget_Renderer_Color(cr["black"]));
	tb2.give_BG_Renderer(new Widget_Renderer_Color(cr["black"]));

	//Setup actions
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_RETURN), 1);	//ENTER
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_SPACE), 1);	//ENTER
  set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_START), 1);	//ENTER
  set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_A), 1);	//ENTER

	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), 2);	//Give up
  set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_BACK), 2);	//Give up
  set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_B), 2);	//Give up

	//Load text
	String filepath = "text/Victory.txt";
	char c;
	ifstream fin;
	fin.open(filepath.c_str(), ios::in);
	for(;;) {
		fin.get(c);
		if(!fin)
			break;
		m_text += c;
	}
	fin.close();
	tb.set_text(m_text);
}

VictoryState::~VictoryState()
{
}

void VictoryState::on_cover()
{
}

void VictoryState::on_uncover()
{
}

void VictoryState::on_push()
{
}

void VictoryState::on_pop()
{
}

void VictoryState::on_event(const Zeni::Zeni_Input_ID &/*id*/, const float &confidence, const int &action)	
{
	if (action == 1 || action == 2)
	{
		//ENTER
		if (confidence == 1.0)
			get_Game().pop_state();
	}
}

void VictoryState::perform_logic()
{
}

void VictoryState::step(const float &/*time_step*/)
{
}

void VictoryState::render()
{
	Video &vr = get_Video();
	Colors &cr = get_Colors();

	vr.set_2d(make_pair(Point2f(), Point2f(RES_HORIZ, RES_VERT)), true);
	vr.set_clear_Color(cr["black"]);
	/*get_Fonts()["title"].render_text(m_text,
										Point2f(640.0f, 32.0f),
										cr["white"]
										ZENI_CENTER );*/

	tb.render();
	tb2.render();
}
