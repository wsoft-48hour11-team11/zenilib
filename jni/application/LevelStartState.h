#ifndef LEVELSTARTSTATE_H
#define LEVELSTARTSTATE_H

#include <zenilib.h>

class LevelStartState : public Zeni::Gamestate_II
{
	public:
    LevelStartState(const Zeni::Gamestate &gs);
		~LevelStartState();

		void on_cover();
		void on_uncover();

		void on_push();
		void on_pop();

		void on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action);

		void render();

	private:
    Zeni::Gamestate my_gamestate;
};

#endif
