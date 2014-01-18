#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H
#include <zenilib.h>

class LevelEditor : public Zeni::GameState_II
{
	public:
		LevelEditor();
		~LevelEditor();

		void on_cover();
		void on_uncover();

		void on_push();
		void on_pop();

		void on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action);

		void perform_logic();
		void step(const float &time_step);
		void render();

	private:

};

#endif
