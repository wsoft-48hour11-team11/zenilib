#ifndef COMMON_H
#define COMMON_H

#include <zenilib.h>

enum Tile {TILE_EMPTY = 0,         // A
           TILE_FULL = 1,          // B 
           TILE_UPPER_LEFT = 2,    // C
           TILE_UPPER_RIGHT = 3,   // D
           TILE_LOWER_LEFT = 4,    // E
           TILE_LOWER_RIGHT = 5,   // F
           TILE_DEPOSIT = 6,       // G
           TILE_SPAWN_PLAYER = 7   // H
          };

inline const char * const tile_asset(const Tile &tile) {
  switch(tile) {
    case TILE_SPAWN_PLAYER:
    case TILE_EMPTY:       return 0;
    case TILE_FULL:        return "tile_full";
    case TILE_UPPER_LEFT:  return "tile_upper_left";
    case TILE_UPPER_RIGHT: return "tile_upper_right";
    case TILE_LOWER_LEFT:  return "tile_lower_left";
    case TILE_LOWER_RIGHT: return "tile_lower_right";
    case TILE_DEPOSIT:     return "tile_deposit";
    default:               assert(!"Tile");
                           return 0;
  }
}

typedef std::vector<Tile> Row;

class Grid {
public:
  Grid() {}

  Grid(const Zeni::Point2i &size, const Zeni::Point2i &render_offset)
    : m_grid(size.y, Row(size.x, TILE_FULL)),
    m_render_offset(render_offset)
  {
    build_vbo();
  }

  bool load(Zeni::String filepath);
  bool save(Zeni::String filepath) const;

  size_t get_height() const {return m_grid.size();}
  size_t get_width() const {return m_grid.empty() ? 0 : m_grid[0].size();}

  const Zeni::Point2i & get_spawn_player() const {return m_spawn_player;}

  template <typename INDEX>
  const Row & operator[](const INDEX &index) const {return m_grid[index];}

  template <typename INDEX>
  Row & operator[](const INDEX &index) {return m_grid[index];}

  template <typename INDEX>
  const Row & at(const INDEX &index) const {return m_grid.at(index);}

  template <typename INDEX>
  Row & at(const INDEX &index) {return m_grid.at(index);}

  void update()
  {
	  build_vbo();
  }

  void render() {
    m_grid_buffer->render();
  }

private:
  void build_vbo();

  std::vector<Row> m_grid;
  Zeni::Point2i m_spawn_player;

  Zeni::Point2i m_render_offset;
  std::shared_ptr<Zeni::Vertex_Buffer> m_grid_buffer;
};

#endif
