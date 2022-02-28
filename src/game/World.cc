
# include "World.hh"

namespace cellify {

  World::World():
    utils::CoreObject("world"),

    m_grid()
  {
    setService("cellify");
  }

  const Grid&
  World::grid() const noexcept {
    return m_grid;
  }

}
