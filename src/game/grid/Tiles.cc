
# include "Tiles.hh"

namespace cellify {

  std::string
  tileToString(const Tile& t) noexcept {
    switch (t) {
      case Tile::Colony:
        return "colony";
      case Tile::Ant:
        return "ant";
      case Tile::Food:
        return "food";
      default:
        return "unknown";
    }
  }

}
