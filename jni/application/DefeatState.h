#ifndef DEFEATSTATE_H
#define DEFEATSTATE_H

#include <zenilib.h>

class DefeatState : public Zeni::Gamestate_II
{
	public:
		DefeatState(const int &level_number);
		~DefeatState();

		void on_cover();
		void on_uncover();

		void on_push();
		void on_pop();

		void on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action);

		void perform_logic();
		void step(const float &time_step);
		void render();

	private:
    int m_level_number;

		Zeni::String m_text;
		
		Zeni::Text_Box tb;
		Zeni::Text_Box tb2;

};

#endif
