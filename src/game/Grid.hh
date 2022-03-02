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

      /**
       * @brief - Returns the element at the specified index.
       * @param id - the index of the element to fetch.
       * @return - the element or an error in case the index
       *           is not valid.
       */
      const Cell&
      at(unsigned id) const;

      /**
       * @brief - Query whether the input cell contains an
       *          element. If not, the return value is a
       *          negative element.
       * @param x - the x coordinate to check.
       * @param y - the y coordinate to check.
       * @param includeMobs - `true` if mobs should also be
       *                      checked when searching for an
       *                      item at the position.
       * @return - a positive or null value indicating the
       *           index to use to query the element at the
       *           specified coordinate or a negative value
       *           in case the cell is empty.
       */
      int
      at(int x, int y, bool includeMobs = false) const noexcept;

      /**
       * @brief - Determine whether the input coordinates are
       *          obstructed. Internally the above method is
       *          called.
       * @param x - the abscissa to check for obstruction.
       * @param y - the ordinate to check for obstruction.
       * @param includeMobs - `true` in case the obstruction
       *                      should also include mobs.
       * @return - `true` if the location is occupied.
       */
      bool
      obstructed(int x, int y, bool includeMobs = false) const noexcept;

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
