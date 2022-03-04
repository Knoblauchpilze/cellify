
# include <Grid.hh>
# include "Ant.hh"

namespace cellify {

  Grid::Grid(utils::RNG& rng):
    utils::CoreObject("grid"),

    m_min(),
    m_max(),

    m_cells()
  {
    setService("game");

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

  unsigned
  Grid::size() const noexcept {
    return m_cells.size();
  }

  Element&
  Grid::at(unsigned id) {
    if (id > m_cells.size()) {
      error(
        "Failed to fetch item " + std::to_string(id),
        "Only " + std::to_string(id) + " registered"
      );
    }

    return *m_cells[id];
  }

  const Element&
  Grid::at(unsigned id) const {
    if (id > m_cells.size()) {
      error(
        "Failed to fetch item " + std::to_string(id),
        "Only " + std::to_string(id) + " registered"
      );
    }

    return *m_cells[id];
  }

  int
  Grid::at(int x, int y, bool includeMobs) const noexcept {
    /// TODO: Optimize the search for elements.
    unsigned id = 0u;

    while (id < m_cells.size()) {
      const ElementShPtr& c = m_cells[id];

      bool coordsMatch = (c->pos().x() == x && c->pos().y() == y);
      if (coordsMatch && (includeMobs || c->type() != Tile::Ant)) {
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
  Grid::spawn(ElementShPtr elem) {
    if (elem == nullptr) {
      error(
        "Failed to spawn element in grid",
        "Invalid null element"
      );
    }

    m_cells.push_back(elem);
  }

  void
  Grid::update() noexcept {
    // Use the standard algorithm to remove elements
    // that have been marked for deletion.
    m_cells.erase(
      std::remove_if(
        m_cells.begin(),
        m_cells.end(),
        [](ElementShPtr el){
          return el->tobeDeleted();
        }
      ),
      m_cells.end()
    );
  }

  void
  Grid::initialize(utils::RNG& /*rng*/) noexcept {
# ifndef SIM
    m_cells.push_back(std::make_shared<Element>(Tile::Ant, utils::Point2i(0, 0), std::make_shared<Ant>()));
# else
    static const int size = 50;
    static const int count = 25;

    for (int id = 0 ; id < count ; ++id) {
      Tile t = static_cast<Tile>(id % 3);

      utils::Point2i pos;
      pos.x() = rng.rndInt(-size / 2, size / 2);
      pos.y() = rng.rndInt(-size / 2, size / 2);

      AIShPtr brain = nullptr;
      if (t == Tile::Ant) {
        brain = std::make_shared<Ant>();
      }

      m_cells.push_back(std::make_shared<Element>(t, pos, brain));
    }
# endif
  }

}
