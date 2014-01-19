#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include <zenilib.h>

#include "common.h"
#include "player.h"
#include "Crawler.h"
#include "DeathRay.h"
#include "PowerSeal.h"
#include "GameSingleton.h"
#include "Portal.h"

class Play_State : public Zeni::Gamestate_II {
	  Play_State(const Play_State &);
	  Play_State operator=(const Play_State &);

	public:
	  Play_State(const int &level_number = 0);

	  ~Play_State();

	  void perform_logic();
	  void step(const float &time_step);

	private:
	  void on_push();
	  void on_cover();
	  void on_uncover();
	  void on_pop();

    void on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action);

	  void prerender();

	  void render();

    int m_level_number;

	  Zeni::Chronometer<Zeni::Time> m_chrono;
		float m_time_passed;
    float m_time_to_process;
    float m_time_processed;
		float m_max_time_step;  //< Optional
		float m_max_time_steps; //< Optional

	  Grid m_grid;
	  Player m_player;

    std::list<PowerSeal> m_power_seals;
    PowerSeal * m_powerseal;

	std::list<Enemy*> m_enemies;
	std::list<DeathRay*> m_deathrays;

    std::shared_ptr<Portal> m_portal;

    std::vector<float> m_time_to_failure;
};

#endif
