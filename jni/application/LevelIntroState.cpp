#include "LevelIntroState.h"

#include <fstream>

#include "common.h"
#include "GameSingleton.h"
#include "Play_State.h"

using namespace std;
using namespace Zeni;

LevelIntroState::LevelIntroState(const int &level)
  : m_level_number(level),
	tb(Point2f(0, 0), Point2f(RES_HORIZ, RES_VERT),"intro", "", get_Colors()["white"]),
	tb2(Point2f(RES_HORIZ - 128.0f, RES_VERT - 32.0f), Point2f(RES_HORIZ, RES_VERT),"intro", "Press Enter", get_Colors()["white"])
{
	//Set up text boxes
	Colors &cr = get_Colors();
	tb.give_BG_Renderer(new Widget_Renderer_Color(cr["black"]));
	tb2.give_BG_Renderer(new Widget_Renderer_Color(cr["black"]));

	//Setup actions
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_RETURN), 1);	//ENTER

	//Load text
	String filepath = "text/LevelIntro" + itoa(level) + ".txt";
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

LevelIntroState::~LevelIntroState()
{
}

void LevelIntroState::on_cover()
{
}

void LevelIntroState::on_uncover()
{
}

void LevelIntroState::on_push()
{
}

void LevelIntroState::on_pop()
{
}

void LevelIntroState::on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action)	
{
	if (action == 1)
	{
		//ENTER
		if (confidence == 1.0)
		{
			get_Game().pop_state();
      get_Game().push_state(new Play_State(m_level_number));
		}
	}
}

void LevelIntroState::perform_logic()
{
}

void LevelIntroState::step(const float &time_step)
{
}

void LevelIntroState::render()
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
