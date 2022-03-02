
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

  const Cell&
  Grid::at(unsigned id) const {
    if (id > m_cells.size()) {
      error(
        "Failed to fetch item " + std::to_string(id),
        "Only " + std::to_string(id) + " registered"
      );
    }

    return m_cells[id];
  }

  int
  Grid::at(int x, int y, bool includeMobs) const noexcept {
    /// TODO: Optimize the search for elements.
    unsigned id = 0u;

    while (id < m_cells.size()) {
      const Cell& c = m_cells[id];

      bool coordsMatch = (c.pos.x() == x && c.pos.y() == y);
      if (coordsMatch && (includeMobs || c.item != Tile::Ant)) {
        return static_cast<int>(id);
      }

      ++id;
    }

    return -1;
  }

  bool
  Grid::obstructed(int x, int y, bool includeMobs) const noexcept {
    return at(x, y, includeMobs) >= 0;
  }

  bool
  Grid::obstructed(const utils::Point2i& p, bool includeMobs) const noexcept {
    return obstructed(p.x(), p.y(), includeMobs);
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
