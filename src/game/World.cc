
# include "World.hh"
# include "Influence.hh"

namespace cellify {

  World::World():
    utils::CoreObject("world"),

    m_rng(),
    m_grid(nullptr),

    m_paused(true),
    m_timestamp(zero())
  {
    setService("cellify");

    // Create the grid.
    m_grid = std::make_shared<Grid>(m_rng);
  }

  const Grid&
  World::grid() const noexcept {
    return *m_grid;
  }

  void
  World::step(float tDelta) {
    // Disable step in case the world is in pause.
    if (m_paused) {
      return;
    }

    // The input delat is expressed in seconds so
    // we need to convert that in milliseconds.
    m_timestamp += 1000.0f * tDelta;

    StepInfo si{
      m_rng,        // rng

      m_timestamp,  // moment
      tDelta,       // elapsed

      *m_grid,      // grid

      Elements(),   // elements

      Influences()  // actions
    };

    // Simulate elements.
    for (unsigned id = 0u ; id < m_grid->size() ; ++id) {
      m_grid->at(id).step(si);
    }

    // Process influences.
    for (unsigned id = 0u ; id < si.spawned.size() ; ++id) {
      m_grid->spawn(si.spawned[id]);
    }

    for (unsigned id = 0u ; id < si.actions.size() ; ++id) {
      si.actions[id]->apply();
    }

    // Perform the update of the grid (this step
    // includes deleting the elements marked for
    // deletion, etc).
    m_grid->update();
  }

  void
  World::pause() {
    // Pause each element if needed.
    if (m_paused) {
      return;
    }

    for (unsigned id = 0u ; id < m_grid->size() ; ++id) {
      m_grid->at(id).pause(m_timestamp);
    }

    m_paused = true;
  }

  void
  World::resume() {
    // Resume each element if needed.
    if (!m_paused) {
      return;
    }

    for (unsigned id = 0u ; id < m_grid->size() ; ++id) {
      m_grid->at(id).resume(m_timestamp);
    }

    m_paused = false;
  }

  unsigned
  World::count(const Tile& tile) const noexcept {
    unsigned count = 0u;

    // Simulate elements.
    for (unsigned id = 0u ; id < m_grid->size() ; ++id) {
      if (tile == m_grid->at(id).type()) {
        ++count;
      }
    }

    return count;
  }

  bool
  World::spawn(const utils::Point2i& p,
               const Tile& tile) noexcept
  {
    ElementShPtr e;

    switch (tile) {
      case Tile::Food:
      case Tile::Obstacle:
        e = std::make_shared<Element>(tile, p);
        break;
      case Tile::Colony:
      case Tile::Ant:
      case Tile::Pheromon:
      default:
        // Do nothing, unsupported spawn request.
        break;
    }

    if (e == nullptr) {
      warn("Unsupported spawn request for " + tileToString(tile));
      return false;
    }

    m_grid->spawn(e);

    return true;
  }

}
