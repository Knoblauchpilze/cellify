#ifndef    GRID_HH
# define   GRID_HH

# include <vector>
# include <memory>
# include <maths_utils/Point2.hh>
# include <core_utils/CoreObject.hh>
# include <core_utils/RNG.hh>
# include "Tiles.hh"
# include "StepInfo.hh"
# include "Element.hh"
# include "Locator.hh"

namespace cellify {

  /// @brief - Convenience define allowing to represent a list
  /// of elements indices.
  using Indices = std::vector<int>;

  class Grid: public utils::CoreObject, public Locator {
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
       * @brief - Returns the number of elements registered in
       *          the grid at the moment.
       * @return - the number of elements defined in the world.
       */
      unsigned
      size() const noexcept;

      /**
       * @brief - Returns the element at the specified index.
       * @param id - the index of the element to fetch.
       * @return - the element or an error in case the index
       *           is not valid.
       */
      Element&
      at(unsigned id);

      /**
       * @brief - Returns the element at the specified index.
       * @param id - the index of the element to fetch.
       * @return - the element or an error in case the index
       *           is not valid.
       */
      const Element&
      at(unsigned id) const;

      /**
       * @brief - Query whether the input cell contains an
       *          element. If not, the return value is a
       *          negative element.
       * @param x - the x coordinate to check.
       * @param y - the y coordinate to check.
       * @param includeNonSolid - `true` in case the obstruction
       *                          should also include non solid
       *                          elements such as mobs and pheromons.
       * @return - a vector containing all the indices of the elements
       *           which currently have the specified position. Might
       *           be empty in case the cell is empty.
       */
      Indices
      at(int x, int y, bool includeNonSolid = false) const noexcept;

      /**
       * @brief - Determine whether the input coordinates are
       *          obstructed. Internally the above method is
       *          called.
       * @param x - the abscissa to check for obstruction.
       * @param y - the ordinate to check for obstruction.
       * @param includeNonSolid - `true` in case the obstruction
       *                          should also include non solid
       *                          elements such as mobs and pheromons.
       * @return - `true` if the location is occupied.
       */
      bool
      obstructed(int x, int y, bool includeNonSolid = false) const noexcept;

      /**
       * @brief - Implementation of the interface method to detect
       *          obstructions on the grid.
       * @param p - the coordinates to check for obstruction.
       * @param includeNonSolid - `true` in case the obstruction
       *                          should also include non solid
       *                          elements such as mobs and pheromons.
       * @return - `true` if the location is occupied.
       */
      bool
      obstructed(const utils::Point2i& p,
                 bool includeNonSolid = false) const noexcept override;

      /**
       * @brief - Implementation of the interface method to find the
       *          visible objects within a certain area.
       * @param p - the center of the area to consider.
       * @param d - the distance within which elements should be.
       * @return - the list of elements that are visible.
       */
      Indices
      visible(const utils::Point2i& p,
              float d) const noexcept override;

      /**
       * @brief - Implementation of the interface method to fetch the
       *          element at the specified index. We return null if
       *          the element doesn't exist.
       * @param id - the index of the element to return.
       * @return - a pointer to the element or null in case if it does
       *           not exist.
       */
      const void*
      get(unsigned id) const noexcept override;

      /**
       * @brief - Spawns a new element in the grid and register
       *          it into the internal structure.
       * @param elem - the element to spawn.
       */
      void
      spawn(ElementShPtr elem);

      /**
       * @brief - Update the grid and remove elements which have
       *          been marked for deletion.
       */
      void
      update() noexcept;

    private:

      /**
       * @brief - Generate a random grid.
       * @param rng - a randomness generator.
       */
      void
      initialize(utils::RNG& rng) noexcept;

      /**
       * @brief - Used to attempt the merge of the input pheromon.
       * @param p - the pheromon to merge.
       * @return - `true` in case the pheromon could be merged.
       */
      bool
      mergePheromon(ElementShPtr p) noexcept;

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
      std::vector<ElementShPtr> m_cells;
  };

  using GridShPtr = std::shared_ptr<Grid>;
}

#endif    /* GRID_HH */
