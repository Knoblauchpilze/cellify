
# include "World.hh"

namespace cellify {

  World::World():
    utils::CoreObject("world"),

    m_rng(),
    m_grid(nullptr),

    m_paused(true)
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

    StepInfo si{
      m_rng,         // rng

      utils::now(),  // moment
      tDelta,        // elapsed

      *m_grid,       // grid

      Elements()     // elements
    };

    // Simulate elements.
    for (unsigned id = 0u ; id < m_grid->size() ; ++id) {
      m_grid->at(id).step(si);
    }

    // Process influences.
    for (unsigned id = 0u ; id < si.spawned.size() ; ++id) {
      m_grid->spawn(si.spawned[id]);
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

    utils::TimeStamp n = utils::now();

    for (unsigned id = 0u ; id < m_grid->size() ; ++id) {
      m_grid->at(id).pause(n);
    }

    m_paused = true;
  }

  void
  World::resume() {
    // Resume each element if needed.
    if (!m_paused) {
      return;
    }

    utils::TimeStamp n = utils::now();

    for (unsigned id = 0u ; id < m_grid->size() ; ++id) {
      m_grid->at(id).resume(n);
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

}
