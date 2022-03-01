#ifndef    TILES_HH
# define   TILES_HH

# include <vector>
# include <maths_utils/Point2.hh>

namespace cellify {

  /// @brief - Definition of the possible tiles for the
  /// world in the game.
  enum class Tile {
    Colony,
    Ant,
    Food
  };

  /// @brief - A convenience define representing a tile and
  /// its position.
  struct Cell {
    // The type of the item.
    Tile item;

    // The position of the item.
    utils::Point2i pos;
  };

  /// @brief - Convenience define to handle a list of cells.
  using Cells = std::vector<Cell>;

}

#endif    /* TILES_HH */
