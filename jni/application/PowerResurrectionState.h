#ifndef POWERRESURRECTIONSTATE_H
#define POWERRESURRECTIONSTATE_H

#include <zenilib.h>
#include <map>


#include "cursor.h"
#include "Play_State.h"
#include "player.h"
#include "PowerSeal.h"

class PowerResurrectionState : public Zeni::Gamestate_II
{
	public:
		PowerResurrectionState(Zeni::Gamestate gamestate, const int &level_number, Player* player, std::map<Power, Zeni::Point2f> power_locations, Zeni::Vector2f grid_offset);
		~PowerResurrectionState();

		void on_cover();
		void on_uncover();

		void on_push();
		void on_pop();

		void on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action);

		void perform_logic();
		void step(const float &time_step);
		void render();

	private:
		PowerResurrectionState();

		Zeni::Chronometer<Zeni::Time> m_chrono;
		float m_time_passed;
    float m_time_to_process;
    float m_time_processed;
		float m_max_time_step;  //< Optional
		float m_max_time_steps; //< Optional

		Zeni::Gamestate m_game_state;
		int m_level_number;
		Player* m_player;
		std::map<Power, Zeni::Point2f> m_power_locations;
		Zeni::Vector2f m_grid_offset;

		bool m_game_over;
		float m_death_countdown;

		float m_fade;
};

#endif
