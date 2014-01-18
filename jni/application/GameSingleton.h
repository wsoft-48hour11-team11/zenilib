#ifndef GAMESINGLETON_H
#define GAMESINGLETON_H

#include <zenilib.h>

class GameSingleton;

class GameSingleton
{
	public:
		static GameSingleton* getInstance();

		void loadLevelList(Zeni::String filepath);

		std::vector<Zeni::String> level_list;

	private:
		GameSingleton()
		{
		}

		static GameSingleton* m_instance;
};

#endif
