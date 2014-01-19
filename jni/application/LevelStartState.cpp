#include "LevelStartState.h"

#include <fstream>

#include "common.h"
#include "GameSingleton.h"
#include "Play_State.h"

using namespace std;
using namespace Zeni;

LevelStartState::LevelStartState(const Zeni::Gamestate &gs)
  : my_gamestate(gs)
{
	//Setup actions
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), 3);	//ESCAPE
  set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_BACK), 3);	//ENTER
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_RETURN), 1);	//ENTER
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_SPACE), 1);	//ENTER
  set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_START), 1);	//ENTER
  set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_A), 1);	//ENTER
}

LevelStartState::~LevelStartState()
{
}

void LevelStartState::on_cover()
{
}

void LevelStartState::on_uncover()
{
}

void LevelStartState::on_push()
{
}

void LevelStartState::on_pop()
{
}

void LevelStartState::on_event(const Zeni::Zeni_Input_ID &/*id*/, const float &confidence, const int &action)	
{
	if (action == 1)
	{
		//ENTER
		if (confidence == 1.0)
		{
			get_Game().pop_state();
		}
	}
	if (action == 3)
	{
		//ENTER
		if (confidence == 1.0)
		{
      get_Game().push_Popup_Menu_State();
		}
	}
}

void LevelStartState::render()
{
  my_gamestate.render();

  Font &fr = get_Fonts()["main_game"];

  fr.render_text("Press Enter to start!", Point2f(RES_HORIZ / 2.0f, RES_VERT - fr.get_text_height() - 10.0f), get_Colors()["start_color"], ZENI_CENTER);
}
