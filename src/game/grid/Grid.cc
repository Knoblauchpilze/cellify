
# include <Grid.hh>
# include "Colony.hh"
# include "Ant.hh"
# include "Pheromon.hh"

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

  Indices
  Grid::at(int x, int y, bool includeNonSolid) const noexcept {
    /// TODO: Optimize the search for elements.
    Indices out;

    unsigned id = 0u;

    while (id < m_cells.size()) {
      const ElementShPtr& c = m_cells[id];

      bool coordsMatch = (c->pos().x() == x && c->pos().y() == y);
      if (coordsMatch) {
        bool isSolid = c->type() == Tile::Colony || c->type() == Tile::Food;

        if (includeNonSolid || isSolid) {
          out.push_back(static_cast<int>(id));
        }
      }

      ++id;
    }

    return out;
  }

  bool
  Grid::obstructed(int x, int y, bool includeNonSolid) const noexcept {
    return !at(x, y, includeNonSolid).empty();
  }

  bool
  Grid::obstructed(const utils::Point2i& p, bool includeNonSolid) const noexcept {
    return obstructed(p.x(), p.y(), includeNonSolid);
  }

  void
  Grid::spawn(ElementShPtr elem) {
    if (elem == nullptr) {
      error(
        "Failed to spawn element in grid",
        "Invalid null element"
      );
    }

    // If the element is a pheromon, try to merge it
    // with existing elements. We can only do that if
    // the pheromons have matching types.
    if (elem->type() == Tile::Pheromon) {
      if (mergePheromon(elem)) {
        return;
      }
    }

    log(
      "Spawning agent with type " + tileToString(elem->type()) +
      " at " + elem->pos().toString(),
      utils::Level::Info
    );

    m_cells.push_back(elem);
  }

  void
  Grid::update() noexcept {
    // Use the standard algorithm to remove elements
    // that have been marked for deletion.
    unsigned sz = m_cells.size();

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

    if (sz != m_cells.size()) {
      log("Removed " + std::to_string(sz - m_cells.size()) + " agent(s)");
    }
  }

  void
  Grid::initialize(utils::RNG& /*rng*/) noexcept {
    // Generate an anthill at the origin of the world.
    m_cells.push_back(std::make_shared<Element>(
      Tile::Colony, utils::Point2i(), std::make_shared<Colony>()
    ));

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
  }

  bool
  Grid::mergePheromon(ElementShPtr p) noexcept {
    // Get elements at the position of the pheromon.
    Indices ids = at(p->pos().x(), p->pos().y(), true);

    // If there are none, nothing to merge.
    if (ids.empty()) {
      return false;
    }

    Scent toMerge = *reinterpret_cast<const Scent*>(p->data());

    // Check for a pheromon with the same scent.
    unsigned id = 0u;

    while (id < ids.size()) {
      Element& ex = at(ids[id]);

      // We have to have a pheromon with the same scent.
      if (ex.type() == Tile::Pheromon) {
        Scent existingScent = *reinterpret_cast<const Scent*>(ex.data());

        if (toMerge == existingScent) {
          // Merge both elements.
          ex.merge(*p);

          return true;
        }
      }

      ++id;
    }

    // We didn't merge the pheromon.
    return false;
  }

}
