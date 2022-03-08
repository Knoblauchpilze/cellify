
# include "Ant.hh"
# include "AStar.hh"
# include "Pheromon.hh"

/// @brief - Define the radius into which ants are
/// allowed to move.
# define TARGET_RADIUS 10

/// @brief - The interval between emitting a new
/// pheromone. Expressed in milliseconds.
# define PHEROMON_SPAWN_INTERVAL 500

namespace cellify {

  Ant::Ant():
    AI("ant"),

    m_lastPheromon()
  {}

  void
  Ant::init(Info& info) {
    generatePath(info);

    m_lastPheromon = info.moment;
  }

  void
  Ant::step(Info& info) {
    // Update the position based on whether we have
    // a path or not.
    if (info.path.empty()) {
      generatePath(info);
    }

    // Emit a pheromon if possible.
    if (m_lastPheromon + millisecondsToDuration(PHEROMON_SPAWN_INTERVAL) < info.moment) {
      spawnPheromon(info);
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

  void
  Ant::spawnPheromon(Info& info) noexcept {
    info.spawned.push_back(Animat{
      info.pos,
      std::make_shared<Pheromon>(1.0f, 0.1f)
    });

    // Update the variables tracking the spawn of a new
    // pheromone.
    m_lastPheromon = info.moment;
  }

}
