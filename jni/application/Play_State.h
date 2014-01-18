#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include <zenilib.h>

#include "common.h"
#include "player.h"
#include "Crawler.h"
#include "GameSingleton.h"

class Play_State : public Zeni::Gamestate_II {
	  Play_State(const Play_State &);
	  Play_State operator=(const Play_State &);

	public:
	  Play_State();

	  ~Play_State();

	  void perform_logic();
	  void step(const float &time_step);

	private:
	  void on_push();

	  void on_pop();

	  void on_cover();

    void on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action);

	  void prerender();

	  void render();

	  Zeni::Time m_current_time;
		float m_time_passed;
		float m_max_time_step;  //< Optional
		float m_max_time_steps; //< Optional

	  Grid m_grid;
	  Player m_player;

	  Crawler m_crawler;
};

#endif
