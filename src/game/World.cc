
# include "World.hh"

namespace cellify {

  World::World():
    utils::CoreObject("world"),

    m_rng(),
    m_grid(nullptr)
  {
    setService("cellify");

    // Create the grid.
    m_grid = std::make_shared<Grid>(m_rng);
  }

  const Grid&
  World::grid() const noexcept {
    return *m_grid;
  }

}
