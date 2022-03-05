
# include "Ant.hh"
# include "AStar.hh"

/// @brief - Define the radius into which ants are
/// allowed to move.
# define TARGET_RADIUS 10

namespace cellify {

  Ant::Ant():
    AI("ant")
  {}

  void
  Ant::init(Info& info) {
    generatePath(info);
  }

  void
  Ant::step(Info& info) {
    // Update the position based on whether we have
    // a path or not.
    if (info.path.empty()) {
      generatePath(info);
    }
  }

  bool
  Ant::generatePath(Info& info) {
    // Pick a random target and find a path to it.
    int x = info.rng.rndInt(info.pos.x() - TARGET_RADIUS, info.pos.x() + TARGET_RADIUS);
    int y = info.rng.rndInt(info.pos.y() - TARGET_RADIUS, info.pos.y() + TARGET_RADIUS);

    AStar astar(info.pos, utils::Point2i(x, y), info.locator);
    bool ok = astar.findPath(info.path, -1.0f, false);
    if (!ok) {
      warn("Failed to find a path for the and");
    }

    if (ok) {
      log("Moving from " + info.path.begin().toString() + " to " + info.path.end().toString());
    }

    return ok;
  }

}
