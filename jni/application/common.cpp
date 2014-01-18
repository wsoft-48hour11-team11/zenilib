#include "common.h"

#include <fstream>

using namespace Zeni;
using namespace std;

bool load_level_to_grid(Grid& grid, String filepath)
{
	ifstream fin;
	fin.open(filepath.c_str(), ios::in);
	if (!fin.is_open())
	{
		String message = "Unable to open file \"" + filepath + "\"!";
		cerr << message << endl;
		return false;
	}
	else
	{
		try
		{
			char delim;
			int row = 0;
			int col = 0;
			int id;
			do
			{
				fin >> id;
				fin.get(delim);
				if (delim == ',')
				{
					grid[row][col] = static_cast<Tile>(id);
					col++;
				}
				else if (delim == '\r')
				{
					grid[row][col] = static_cast<Tile>(id);
					row++;
					col = 0;
				}
			} while (id != -1);
		}
		catch(...)
		{
			String message = "Error loading file \"" + filepath + "\"!";
			cerr << message << endl;
			return false;
		}
	}
	return true;
}

bool save_level_to_file(Grid& grid, String filepath)
{
	ofstream fout;
	fout.open(filepath.c_str(), ios::out);
	for (unsigned int i = 0; i < grid.get_height(); i++)
	{
		for (unsigned int j = 0; j < grid.get_width(); j++)
		{
			fout << grid[i][j] << ",";
		}
		fout << "\r";
	}
	fout << -1 << "@\r";
	fout.close();
	return true;
}
