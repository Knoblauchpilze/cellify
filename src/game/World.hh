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

      /**
       * @brief - Used to move one step ahead in time in this
       *          world, given that `tDelta` represents the
       *          duration of the last frame in seconds.
       * @param tDelta - the duration of the last frame in
       *                 seconds.
       */
      void
      step(float tDelta);

    private:

      /**
       * @brief - The random number generator used by this
       *          world.
       */
      utils::RNG m_rng;

      /**
       * @brief - The grid associated to the world.
       */
      GridShPtr m_grid;
  };

  using WorldShPtr = std::shared_ptr<World>;
}

#endif    /* WORLD_HH */
