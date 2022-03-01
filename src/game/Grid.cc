
# include <Grid.hh>

namespace cellify {

  Grid::Grid(utils::RNG& rng):
    utils::CoreObject("grid"),

    m_min(),
    m_max(),

    m_cells()
  {
    setService("cellify");

    initialize(rng);
  }

  utils::Point2i
  Grid::min() const noexcept {
    return m_min;
  }

  utils::Point2i
  Grid::max() const noexcept {
    return m_max;
  }

  const Cells&
  Grid::cells() const noexcept {
    return m_cells;
  }

  void
  Grid::initialize(utils::RNG& rng) noexcept {
    static const int size = 50;
    static const int count = 25;

    for (int id = 0 ; id < count ; ++id) {
      Cell c;

      c.item = static_cast<Tile>(id % 3);

      c.pos.x() = rng.rndInt(-size / 2, size / 2);
      c.pos.y() = rng.rndInt(-size / 2, size / 2);

      m_cells.push_back(c);
    }
  }

}
