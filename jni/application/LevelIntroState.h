#ifndef LEVELINTROSTATE_H
#define LEVELINTROSTATE_H

#include <zenilib.h>

class LevelIntroState : public Zeni::Gamestate_II
{
	public:
		LevelIntroState(const int &level = 0);
		~LevelIntroState();

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
