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
    Food,
    Pheromon,
    Obstacle
  };

  /**
   * @brief - Convert a tile to its string representation.
   * @param t - the tile to convert.
   * @return - the string representation of the tile.
   */
  std::string
  tileToString(const Tile& t) noexcept;

}

#endif    /* TILES_HH */
