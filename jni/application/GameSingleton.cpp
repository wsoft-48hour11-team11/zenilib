#include <zenilib.h>

#include "GameSingleton.h"

#include <fstream>

using namespace std;
using namespace Zeni;

GameSingleton* GameSingleton::m_instance = 0;

GameSingleton* GameSingleton::getInstance()
{
	if (!m_instance)
	{
		m_instance = new GameSingleton();
		m_instance->loadLevelList("levellist.txt");
	}
	return m_instance;
}

void GameSingleton::loadLevelList(Zeni::String filepath)
{
	ifstream fin;
	fin.open(filepath.c_str(), ios::in);
	if (fin.is_open())
	{
		level_list.clear();
		String line;
		char c;
		while (fin.get(c))
		{
			if (c == '\n')
			{
				level_list.push_back(line);
				line.clear();
			}
			else
			{
				line += c;
			}
		}
	}
	fin.close();
}
