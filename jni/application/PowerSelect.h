#ifndef POWERSELECT_H
#define POWERSELECT_H

#include <zenilib.h>

#include "cursor.h"
#include "Play_State.h"
#include "player.h"
#include "PowerSeal.h"

class PowerSelect : public Zeni::Gamestate_II
{
	public:
		PowerSelect(Zeni::Gamestate gamestate, Player* player, PowerSeal* powerseal);
		~PowerSelect();

		void on_cover();
		void on_uncover();

		void on_push();
		void on_pop();

		void on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action);

		void perform_logic();
		void step(const float &time_step);
		void render();

	private:
		PowerSelect();

		Zeni::Gamestate m_game_state;
		Player* m_player;
		PowerSeal* m_powerseal;
		Cursor m_cursor;

		int m_cursor_index;
		int m_max_cursor_index;
		Power m_highlighted_power;

		Zeni::Text_Box tb;
		Zeni::Text_Box tb2;
};

#endif
