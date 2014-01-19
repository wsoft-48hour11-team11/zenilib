#include "common.h"

/** TODO:
 *   -Set up the failure timer (and add time as powers are deposited (and remove it when they're retrieved!))
 *   -Play music and sound effects
 *   -Make art assets consistent
 *   -Connect levels
 */

#include <fstream>

using namespace Zeni;
using namespace std;

bool Grid::load(String filepath)
{
  const size_t width = get_width();
	ifstream fin(filepath.c_str());

  size_t i = 0, j = 0;
  char c;

  while(fin >> c) {
    if(isspace(c))
      continue;

    at(j).at(i) = Tile(c - 'A');

    switch(at(j).at(i)) {
      case TILE_SPAWN_PLAYER:
        m_spawn_player = Point2i(i, j);
        break;

      case TILE_DEPOSIT:
        m_deposits.push_back(Point2i(i, j));
        break;

	  case TILE_SPAWN_CRAWLER:
        m_crawlers.push_back(Point2i(i, j));
        break;

      default:
        break;
    }

    if(++i == width) {
      ++j;
      i = 0;
    }
  }

  if(fin.eof()) {
    build_vbo();
    return true;
  }

  return false;
}

bool Grid::save(String filepath) const
{
  const size_t width = get_width();
  const size_t height = get_height();
	ofstream fout(filepath.c_str());

	for(unsigned int j = 0; j < height; ++j) {
		for(unsigned int i = 0; i < width; ++i)
      fout << char('A' + m_grid[j][i]);
    fout << std::endl;
	}

	return fout.good();
}

void Grid::build_vbo() {
  const size_t width = get_width();
  const size_t height = get_height();

  m_grid_buffer = std::make_shared<Zeni::Vertex_Buffer>();

  for(size_t j = 0; j != height; ++j) {
    for(size_t i = 0; i != width; ++i) {
      const char * const asset = tile_asset(m_grid[j][i], m_in_editor);

      if(asset) {
        Zeni::Vertex2f_Texture v0(Zeni::Point2f(m_render_offset.x + TILE_SIZE * (i + 0.0f), m_render_offset.y + TILE_SIZE * (j + 0.0f)), Zeni::Point2f(0.0f, 0.0f));
        Zeni::Vertex2f_Texture v1(Zeni::Point2f(m_render_offset.x + TILE_SIZE * (i + 0.0f), m_render_offset.y + TILE_SIZE * (j + 1.0f)), Zeni::Point2f(0.0f, 1.0f));
        Zeni::Vertex2f_Texture v2(Zeni::Point2f(m_render_offset.x + TILE_SIZE * (i + 1.0f), m_render_offset.y + TILE_SIZE * (j + 1.0f)), Zeni::Point2f(1.0f, 1.0f));
        Zeni::Vertex2f_Texture v3(Zeni::Point2f(m_render_offset.x + TILE_SIZE * (i + 1.0f), m_render_offset.y + TILE_SIZE * (j + 0.0f)), Zeni::Point2f(1.0f, 0.0f));

        Zeni::Quadrilateral<Zeni::Vertex2f_Texture> quad(v0, v1, v2, v3);
        Zeni::Material mat(asset);
        quad.lend_Material(&mat);

        m_grid_buffer->fax_Quadrilateral(&quad);
      }
    }
  }
}