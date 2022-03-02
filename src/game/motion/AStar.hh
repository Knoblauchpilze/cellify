#ifndef    ASTAR_HH
# define   ASTAR_HH

# include <core_utils/CoreObject.hh>
# include "Path.hh"
# include "Grid.hh"

namespace cellify {

  /// @brief - Forward declaration of the AStarNodes.
  class AStarNodes;

  class AStar: public utils::CoreObject {
    public:

      /**
       * @brief - Create a new A* capable object allowing to
       *          go from the starting point `s` to the end
       *          point `e` using the input grid for collision
       *          checks.
       * @param s - the starting position.
       * @param e - the end position.
       * @param grid - the grid defining obstacles.
       */
      AStar(const utils::Point2i& s,
            const utils::Point2i& e,
            GridShPtr grid);

      /**
       * @brief - Used to generate the path from the starting position
       *          to the end and return the path that was built in the
       *          output argument.
       *          In case no path can be found, the return value will
       *          indicate so.
       * @param path - the path generated to reach the two end points.
       * @param radius - a value indicating how far from the starting
       *                 point the path is allowed to go and still be
       *                 declared valid. Any path that is led to go
       *                 farther from the start is considered to not be
       *                 realistic for the entity to follow.
       * @param allowLog - `true` if the process should be logged.
       * @return - `true` if a path could be find.
       */
      bool
      findPath(Path& path,
               float radius = -1.0f,
               bool allowLog = false) const noexcept;

    private:

      /**
       * @brief - Reconstruct the path from the data which was generated
       *          when exploring the nodes.
       * @param path - the path to update.
       * @param nodes - information about the nodes explores by the algo.
       * @param radius - the maximum distance allowed for any point of
       *                 the path compared to its starting position.
       * @param allowLog - whether or not logs are allowed.
       * @return - whether or not the path could be reconstructed.
       */
      bool
      reconstruct(Path& path, const AStarNodes& nodes, float radius, bool allowLog) const noexcept;

    private:

      /**
       * @brief - The starting point of the algorithm.
       */
      utils::Point2i m_start;

      /**
       * @brief - The end point of the algorithm.
       */
      utils::Point2i m_end;

      /**
       * @brief - A grid allowing to determine whether a cell is
       *          obstructed.
       */
      GridShPtr m_grid;
  };

}

#endif    /* ASTAR_HH */
