#ifndef COMMON_H
#define COMMON_H

#include <zenilib.h>

enum Power {POWER_EMPTY = 0,
            POWER_BLOOD = 1,
            POWER_DEATHRAY = 2,
            POWER_SHADOW = 3,
            POWER_SPEED = 4,
            POWER_TELEPORT = 5,
            POWER_LIST_SIZE = 6
          };

inline const char * const power_asset(const Power &power) {
  switch(power) {
    case POWER_EMPTY:          return "tile_deposit";
    case POWER_BLOOD:          return "power-blood";
    case POWER_DEATHRAY:       return "power-deathray";
    case POWER_SHADOW:         return "power-shadow";
    case POWER_SPEED:          return "power-speed";
    case POWER_TELEPORT:       return "power-teleport";
    default:                   assert(!"Power");
                               return 0;
  }
}

enum Resolution {RES_HORIZ = 1280, RES_VERT = 720, TILE_SIZE = 20};

enum Tile {TILE_EMPTY = 0,         // A
           TILE_FULL = 1,          // B 
           TILE_UPPER_LEFT = 2,    // C
           TILE_UPPER_RIGHT = 3,   // D
           TILE_LOWER_LEFT = 4,    // E
           TILE_LOWER_RIGHT = 5,   // F
           TILE_DEPOSIT = 6,       // G
           TILE_SPAWN_PLAYER = 7,  // H
           TILE_SPAWN_CRAWLER = 8  // I
          };

inline const char * const tile_asset(const Tile &tile, const bool &in_editor) {
  switch(tile) {
    case TILE_EMPTY:           return 0;
    case TILE_FULL:            return "tile_full";
    case TILE_UPPER_LEFT:      return "tile_upper_left";
    case TILE_UPPER_RIGHT:     return "tile_upper_right";
    case TILE_LOWER_LEFT:      return "tile_lower_left";
    case TILE_LOWER_RIGHT:     return "tile_lower_right";
    case TILE_DEPOSIT:         return in_editor ? "tile_deposit" : 0;
    case TILE_SPAWN_PLAYER:    return in_editor ? "tile_spawn_player" : 0;
	case TILE_SPAWN_CRAWLER:    return in_editor ? "tile_spawn_crawler" : 0;
    default:                   assert(!"Tile");
                               return 0;
  }
}

typedef std::vector<Tile> Row;

class Grid {
public:
  Grid() {}

  Grid(const Zeni::Point2i &size, const Zeni::Vector2f &render_offset, const bool &in_editor)
    : m_in_editor(in_editor),
    m_grid(size.y, Row(size.x, TILE_FULL)),
    m_render_offset(render_offset)
  {
    build_vbo();
  }

  bool load(Zeni::String filepath);
  bool save(Zeni::String filepath) const;

  size_t get_height() const {return m_grid.size();}
  size_t get_width() const {return m_grid.empty() ? 0 : m_grid[0].size();}
  const Zeni::Vector2f & get_render_offset() const {return m_render_offset;}

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

  const std::list<Zeni::Point2i> & get_deposits() const {
    return m_deposits;
  }

private:
  void build_vbo();

  bool m_in_editor;

  std::vector<Row> m_grid;
  Zeni::Point2i m_spawn_player;

  Zeni::Vector2f m_render_offset;
  std::shared_ptr<Zeni::Vertex_Buffer> m_grid_buffer;

  std::list<Zeni::Point2i> m_deposits;
};

#endif
