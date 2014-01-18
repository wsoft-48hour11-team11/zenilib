#ifndef COMMON_H
#define COMMON_H

#include <zenilib.h>

enum Tile {TILE_EMPTY = 0,
           TILE_FULL = 1,
           TILE_UPPER_LEFT = 2,
           TILE_UPPER_RIGHT = 3,
           TILE_LOWER_LEFT = 4,
           TILE_LOWER_RIGHT = 5,
           TILE_DEPOSIT = 6};

typedef std::vector<Tile> Row;

class Grid {
public:
  Grid() {}

  Grid(const Zeni::Point2i &size)
    : m_grid(size.y, Row(size.x, TILE_FULL))
  {
  }

  size_t get_height() const {return m_grid.size();}
  size_t get_width() const {return m_grid.empty() ? 0 : m_grid[0].size();}

  template <typename INDEX>
  const Row & operator[](const INDEX &index) const {return m_grid[index];}

  template <typename INDEX>
  Row & operator[](const INDEX &index) {return m_grid[index];}

private:
  std::vector<Row> m_grid;
};

bool load_level_to_grid(Grid& grid, Zeni::String filepath);
bool save_level_to_file(Grid& grid, Zeni::String filepath);

#endif
