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

      /**
       * @brief - Used to indicate that the world should be
       *          paused. Time based entities and actions
       *          should take actions to correctly resume at
       *          a later time.
       */
      void
      pause();

      /**
       * @brief - Used to indicate that the world should be
       *          resuming its activity. Time based entities
       *          should take actions to be resuming their
       *          pathes, motions, etc.
       */
      void
      resume();

      /**
       * @brief - The number of tile of a certain type currently
       *          registered in the world.
       * @param tile - the type of tile to count.
       * @return - the number of tiles of the input type.
       */
      unsigned
      count(const Tile& tile) const noexcept;

      /**
       * @brief - Generate a new element with the specified type
       *          at the input position.
       * @param p - the position to create the element.
       * @param tile - the tile to create.
       * @return - `true` if the element could be spawned.
       */
      bool
      spawn(const utils::Point2i& p,
            const Tile& tile) noexcept;

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

      /**
       * @brief - Defines whether this world is paused (i.e. internal
       *          attributes of the mobs/blocks/etc have already been
       *          updated to reflect the pause status) or not. This
       *          allows to react to consecutive pause requests and
       *          prevent weird behaviors to occur.
       */
      bool m_paused;

      /**
       * @brief - The current time elapsed in the simulation. For each
       *          second that passes we make a certain amount of time
       *          pass.
       *          NOTE: This value is expressed in milliseconds.
       */
      float m_timestamp;
  };

  using WorldShPtr = std::shared_ptr<World>;
}

#endif    /* WORLD_HH */
