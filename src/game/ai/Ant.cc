
# include "Ant.hh"
# include "AStar.hh"

/// @brief - Define the radius into which ants are
/// allowed to move.
# define TARGET_RADIUS 10

/// @brief - The interval defining two consecutive
/// moves of an ant in milliseconds.
# define IDLE_TIME 200

namespace cellify {

  Ant::Ant():
    AI("ant"),

    m_path(),
    m_last(utils::now())
  {}

  void
  Ant::init(Info& info) {
    generatePath(info);
  }

  void
  Ant::step(Info& info) {
    // Update the position based on whether we have
    // a path or not.
    if (m_path.empty()) {
      generatePath(info);
    }

    // Pick the next position in the path and advance
    // to this location if we moved long enough in the
    // past.
    utils::Duration d = info.moment - m_last;
    if (utils::toMilliseconds(d) < IDLE_TIME) {
      return;
    }

    info.pos = m_path.advance();
    m_last = info.moment;
  }

  bool
  Ant::generatePath(Info& info) {
    // Pick a random target and find a path to it.
    int x = info.rng.rndInt(info.pos.x() - TARGET_RADIUS, info.pos.x() + TARGET_RADIUS);
    int y = info.rng.rndInt(info.pos.y() - TARGET_RADIUS, info.pos.y() + TARGET_RADIUS);

    AStar astar(info.pos, utils::Point2i(x, y), info.locator);
    bool ok = astar.findPath(m_path, -1.0f, false);
    if (!ok) {
      warn("Failed to find a path for the and");
    }

    if (ok) {
      log("Moving from " + m_path.begin().toString() + " to " + m_path.end().toString());
    }

    m_last = info.moment;

    return ok;
  }

}
