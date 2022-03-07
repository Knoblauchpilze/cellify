
# include <Grid.hh>
# include "Ant.hh"

/// @brief - The radius of the food circle around the
/// central colony.
# define FOOD_DEPOSITS_RADIUS 10

/// @brief - The number of deposits to put on the food
/// circle around the colony.
# define FOOD_DEPOSITS_COUNT 4

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
    // Generate an anthill at the origin of the world.
    m_cells.push_back(
      std::make_shared<Element>(
        Tile::Colony,
        utils::Point2i(),
        nullptr
      )
    );

    // Generate a ring of food around it with a certain
    // radius.
    for (unsigned id = 0u ; id < FOOD_DEPOSITS_COUNT ; ++id) {
      float perc = 1.0f * id / FOOD_DEPOSITS_COUNT;

      float fx = FOOD_DEPOSITS_RADIUS * std::cos(2.0f * M_PI * perc);
      float fy = FOOD_DEPOSITS_RADIUS * std::sin(2.0f * M_PI * perc);

      int x = static_cast<int>(std::round(fx));
      int y = static_cast<int>(std::round(fy));

      m_cells.push_back(
        std::make_shared<Element>(
          Tile::Food,
          utils::Point2i(x, y),
          nullptr
        )
      );
    }

    // Generate a ring of ants around the central colony.
    Tile t = Tile::Ant;

    utils::Point2i p(1, 0);
    AIShPtr ai = std::make_shared<Ant>();
    ElementShPtr e = std::make_shared<Element>(t, p, ai);
    m_cells.push_back(e);

// # define ALL_ANTS
# ifdef ALL_ANTS
    p = utils::Point2i(-1, 0);
    ai = std::make_shared<Ant>();
    e = std::make_shared<Element>(t, p, ai);
    m_cells.push_back(e);

    p = utils::Point2i(0, 1);
    ai = std::make_shared<Ant>();
    e = std::make_shared<Element>(t, p, ai);
    m_cells.push_back(e);

    p = utils::Point2i(0, -1);
    ai = std::make_shared<Ant>();
    e = std::make_shared<Element>(t, p, ai);
    m_cells.push_back(e);
# endif
  }

}
