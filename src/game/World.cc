
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
  World::step(float /*tDelta*/) {
    // Disable step in case the world is in pause.
    if (m_paused) {
      return;
    }

    /// TODO: Handle this.
  }

  void
  World::pause() {
    // Pause each element if needed.
    if (m_paused) {
      return;
    }

    /// TODO: Handle this.

    m_paused = true;
  }

  void
  World::resume() {
    // Resume each element if needed.
    if (!m_paused) {
      return;
    }

    /// TODO: Handle this.

    m_paused = false;
  }

}
