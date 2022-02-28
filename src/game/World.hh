#ifndef    WORLD_HH
# define   WORLD_HH

# include <memory>
# include "Grid.hh"

namespace cellify {

  class World: public utils::CoreObject {
    public:

      /**
       * @brief - Creates a new infinite grid with no elements.
       */
      World();

      /**
       * @brief - Returns the grid attached to the world.
       * @return - the grid representing this world.
       */
      const Grid&
      grid() const noexcept;

    private:

      /**
       * @brief - The grid associated to the world.
       */
      Grid m_grid;
  };

  using WorldShPtr = std::shared_ptr<World>;
}

#endif    /* WORLD_HH */