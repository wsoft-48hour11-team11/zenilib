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

inline const char * const tile_asset(const Tile &tile) {
  switch(tile) {
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
    : m_grid(size.y, Row(size.x, TILE_FULL))
  {
    const size_t width = get_width();
    const size_t height = get_height();

    for(size_t j = 0; j != height; ++j) {
      for(size_t i = 0; i != width; ++i) {
        const char * const asset = tile_asset(m_grid[j][i]);

        if(asset) {
          Zeni::Vertex2f_Texture v0(Zeni::Point2f(render_offset.x + i + 0.0f, render_offset.y + j + 0.0f), Zeni::Point2f(0.0f, 0.0f));
          Zeni::Vertex2f_Texture v1(Zeni::Point2f(render_offset.x + i + 0.0f, render_offset.y + j + 1.0f), Zeni::Point2f(0.0f, 1.0f));
          Zeni::Vertex2f_Texture v2(Zeni::Point2f(render_offset.x + i + 1.0f, render_offset.y + j + 1.0f), Zeni::Point2f(1.0f, 1.0f));
          Zeni::Vertex2f_Texture v3(Zeni::Point2f(render_offset.x + i + 1.0f, render_offset.y + j + 0.0f), Zeni::Point2f(1.0f, 0.0f));

          Zeni::Quadrilateral<Zeni::Vertex2f_Texture> quad(v0, v1, v2, v3);
          Zeni::Material mat(asset);
          quad.lend_Material(&mat);

          m_grid_buffer.fax_Quadrilateral(&quad);
        }
      }
    }
  }

  size_t get_height() const {return m_grid.size();}
  size_t get_width() const {return m_grid.empty() ? 0 : m_grid[0].size();}

  template <typename INDEX>
  const Row & operator[](const INDEX &index) const {return m_grid[index];}

  template <typename INDEX>
  Row & operator[](const INDEX &index) {return m_grid[index];}

  void render() {
    m_grid_buffer.render();
  }

private:
  std::vector<Row> m_grid;
  Zeni::Vertex_Buffer m_grid_buffer;
};

#endif
