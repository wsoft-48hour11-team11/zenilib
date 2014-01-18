#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include <zenilib.h>

#include "common.h"
#include "player.h"
#include "GameSingleton.h"

class Play_State : public Zeni::Gamestate_II {
	  Play_State(const Play_State &);
	  Play_State operator=(const Play_State &);

	public:
	  Play_State();

	  ~Play_State();

	private:
	  void on_push();

	  void on_pop();

	  void on_cover();

	  void on_controller_button(const SDL_ControllerButtonEvent &event);

	  void prerender();

	  void render();

	  Grid m_grid;
	  Player m_player;
};

#endif
