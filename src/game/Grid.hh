#ifndef    GRID_HH
# define   GRID_HH

# include <vector>
# include <memory>
# include <maths_utils/Point2.hh>
# include <core_utils/CoreObject.hh>
# include <core_utils/RNG.hh>
# include "Tiles.hh"

namespace cellify {

  class Grid: public utils::CoreObject {
    public:

      /**
       * @brief - Creates a new infinite grid with no elements.
       * @param rng - a random number generator to use to create
       *              the grid and initialize it.
       */
      Grid(utils::RNG& rng);

      /**
       * @brief - Returns the minimum extent of the grid.
       * @return - the minimum point reached by an element in the
       *           grid.
       */
      utils::Point2i
      min() const noexcept;

      /**
       * @brief - Returns the maximum extent of the grid.
       * @return - the maximum point reached by an element in the
       *           grid.
       */
      utils::Point2i
      max() const noexcept;

      /**
       * @brief - Returns the list of cells defined in the world.
       * @return - the list of cells defined in the world.
       */
      const Cells&
      cells() const noexcept;

    private:

      /**
       * @brief - Generate a random grid.
       * @param rng - a randomness generator.
       */
      void
      initialize(utils::RNG& rng) noexcept;

    private:

      /**
       * @brief - The minimum coordinates reached by an element of
       *          the grid. This value is updated whenever an item
       *          changes its position.
       */
      utils::Point2i m_min;

      /**
       * @brief - The maximum coordinates reached by an element of
       *          the grid. This value is updated whenever an item
       *          changes its position.
       */
      utils::Point2i m_max;

      /**
       * @brief - The list of elements registered in the grid.
       */
      std::vector<Cell> m_cells;
  };

  using GridShPtr = std::shared_ptr<Grid>;
}

#endif    /* GRID_HH */
