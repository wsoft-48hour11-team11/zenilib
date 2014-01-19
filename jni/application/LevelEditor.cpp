#include "LevelEditor.h"

#include "GameSingleton.h"
#include "LevelIntroState.h"
#include "Play_State.h"

using namespace Zeni;
using namespace std;

LevelEditor::LevelEditor()
	: m_level(Point2i(50, 32), Vector2f(), true)
{
	m_grid_cursor_pos = Point2i(0, 0);
	m_cursor.setDimensions(TILE_SIZE, TILE_SIZE);
	m_cursor.setThickness(4);
	m_level.load("test_level.txt");

	//Setup actions
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_LEFT), 1);	//DPAD_LEFT
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_RIGHT), 2);	//DPAD_RIGHT
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_UP), 3);		//DPAD_UP
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_DOWN), 4);	//DPAD_DOWN
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_1), 17);	//TILE_EMPTY
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_2), 18);	//TILE_FULL
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_3), 19);	//TILE_UPPER_LEFT
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_4), 20);	//TILE_UPPER_RIGHT
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_5), 21);	//TILE_LOWER_LEFT
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_6), 22);	//TILE_LOWER_RIGHT
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_7), 23);	//TILE_DEPOSIT
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_8), 24);	//TILE_SPAWN_PLAYER
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_s), 25);	//Save Level
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_p), 26);	//Play Level
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_c), 27);	//Clear Level
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), 28);	//Popup menu
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_q), 29);	//TILE_SPAWN_CRAWLER
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_i), 30);	//Show Intro Text
}

LevelEditor::~LevelEditor()
{
	GameSingleton::getInstance()->loadLevelList("levellist.txt");
}

void LevelEditor::on_cover()
{
}

void LevelEditor::on_uncover()
{
}

void LevelEditor::on_push()
{
}

void LevelEditor::on_pop()	
{
}

void LevelEditor::on_event(const Zeni::Zeni_Input_ID &/*id*/, const float &confidence, const int &action)	
{
	if (action == 1)
	{
		//DPAD_LEFT
		if (confidence == 1.0)
		{
			if (m_grid_cursor_pos.x > 0)
			{
				--m_grid_cursor_pos.x;
			}
		}
	}
	else if (action == 2)
	{
		//DPAD_RIGHT
		if (confidence == 1.0)
		{
			if (m_grid_cursor_pos.x < int(m_level.get_width() - 1))
			{
				++m_grid_cursor_pos.x;
			}
		}
	}
	else if (action == 3)
	{
		//DPAD_UP
		if (confidence == 1.0)
		{
			if (m_grid_cursor_pos.y > 0)
			{
				--m_grid_cursor_pos.y;
			}
		}
	}
	else if (action == 4)
	{
		//DPAD_DOWN
		if (confidence == 1.0)
		{
			if (m_grid_cursor_pos.y < int(m_level.get_height() - 1))
			{
				++m_grid_cursor_pos.y;
			}
		}
	}
	else if (action == 17)
	{
		//TILE_EMPTY
		if (confidence == 1.0)
		{
			m_level[m_grid_cursor_pos.y][m_grid_cursor_pos.x] = TILE_EMPTY;
			m_level.update();
		}
	}
	else if (action == 18)
	{
		//TILE_FULL
		if (confidence == 1.0)
		{
			m_level[m_grid_cursor_pos.y][m_grid_cursor_pos.x] = TILE_FULL;
			m_level.update();
		}
	}
	else if (action == 19)
	{
		//TILE_UPPER_LEFT
		if (confidence == 1.0)
		{
			m_level[m_grid_cursor_pos.y][m_grid_cursor_pos.x] = TILE_UPPER_LEFT;
			m_level.update();
		}
	}
	else if (action == 20)
	{
		//TILE_UPPER_RIGHT
		if (confidence == 1.0)
		{
			m_level[m_grid_cursor_pos.y][m_grid_cursor_pos.x] = TILE_UPPER_RIGHT;
			m_level.update();
		}
	}
	else if (action == 21)
	{
		//TILE_LOWER_LEFT
		if (confidence == 1.0)
		{
			m_level[m_grid_cursor_pos.y][m_grid_cursor_pos.x] = TILE_LOWER_LEFT;
			m_level.update();
		}
	}
	else if (action == 22)
	{
		//TILE_LOWER_RIGHT
		if (confidence == 1.0)
		{
			m_level[m_grid_cursor_pos.y][m_grid_cursor_pos.x] = TILE_LOWER_RIGHT;
			m_level.update();
		}
	}
	else if (action == 23)
	{
		//TILE_DEPOSIT
		if (confidence == 1.0)
		{
			m_level[m_grid_cursor_pos.y][m_grid_cursor_pos.x] = TILE_DEPOSIT;
			m_level.update();
		}
	}
	else if (action == 24)
	{
		//TILE_SPAWN_PLAYER
		if (confidence == 1.0)
		{
			m_level[m_grid_cursor_pos.y][m_grid_cursor_pos.x] = TILE_SPAWN_PLAYER;
			m_level.update();
		}
	}
	else if (action == 25)
	{
		//Save Level
		if (confidence == 1.0)
		{
			m_level.save("test_level.txt");
		}
	}
	else if (action == 26)
	{
		//Play Level
		if (confidence == 1.0)
		{
			GameSingleton* sing = GameSingleton::getInstance();
			sing->level_list.clear();
			sing->level_list.push_back("test_level.txt");
			get_Game().push_state(new Play_State());
		}
	}
	else if (action == 27)
	{
		//Clear Level
		if (confidence == 1.0)
		{
			for (unsigned int j = 0; j < m_level.get_height(); j++)
			{
				for (unsigned int i = 0; i < m_level.get_width(); i++)
				{
					m_level[j][i] = TILE_FULL;
				}
			}
			m_level.update();
		}
	}
	else if (action == 28)
	{
		//Escape
		if (confidence == 1.0)
		{
			get_Game().push_Popup_Menu_State();
		}
	}
	else if (action == 29)
	{
		//TILE_SPAWN_CRAWLER
		if (confidence == 1.0)
		{
			m_level[m_grid_cursor_pos.y][m_grid_cursor_pos.x] = TILE_SPAWN_CRAWLER;
			m_level.update();
		}
	}
	else if (action == 30)
	{
		//Level Intro
		if (confidence == 1.0)
		{
			GameSingleton* sing = GameSingleton::getInstance();
			sing->level_list.clear();
			sing->level_list.push_back("test_level.txt");
			get_Game().push_state(new LevelIntroState(0));
		}
	}
}

void LevelEditor::perform_logic()
{
}

void LevelEditor::step(const float &/*time_step*/)
{
}

void LevelEditor::render()
{
	Video &vr = get_Video();
	vr.set_clear_Color(Color());
	
	vr.set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(float(TILE_SIZE * m_level.get_width()), float(TILE_SIZE * m_level.get_height()))), true);

	m_level.render();

	m_cursor.setPos(Point2f(float(m_grid_cursor_pos.x * TILE_SIZE), float(m_grid_cursor_pos.y * TILE_SIZE)));
	m_cursor.render();
}
