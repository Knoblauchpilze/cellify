
# include "Ant.hh"
# include "AStar.hh"
# include "Pheromon.hh"

/// @brief - The vision frustim of an ant: defines
/// how far it can perceive blocks. It is also used
/// to define how far a random target can be picked
/// from the current ant's position.
# define ANT_VISION_RADIUS 5

/// @brief - The interval between emitting a new
/// pheromone. Expressed in milliseconds.
# define PHEROMON_SPAWN_INTERVAL 500

namespace cellify {

  std::string
  behaviorToString(const Behavior& b) noexcept {
    switch (b) {
      case Behavior::Wander:
        return "wannder";
      case Behavior::Food:
        return "food";
      case Behavior::Return:
        return "return";
      default:
        return "unknown";
    }
  }

  Ant::Ant():
    AI("ant"),

    m_behavior(Behavior::Wander),
    m_lastPheromon(),
    m_target(nullptr)
  {}

  Behavior
  Ant::mode() const noexcept {
    return m_behavior;
  }

  void
  Ant::init(Info& info) {
    generatePath(info);

    m_lastPheromon = info.moment;

    // The ant is looking for food at first.
    m_behavior = Behavior::Wander;
  }

  void
  Ant::step(Info& info) {
    // Check the behavior and handle the definition of a new
    // target.
    switch (m_behavior) {
      case Behavior::Food:
        food(info);
        break;
      case Behavior::Return:
        returnHome(info);
        break;
      default:
        warn("Unknown behavior " + behaviorToString(m_behavior));
        [[fallthrough]];
      case Behavior::Wander:
        wander(info);
        break;
    }

    // Emit a pheromon if possible.
    if (m_lastPheromon + millisecondsToDuration(PHEROMON_SPAWN_INTERVAL) < info.moment) {
      spawnPheromon(info);
    }
  }

  bool
  Ant::generatePath(Info& info) {
    // Pick a random target and find a path to it if needed.
    if (m_target == nullptr) {
      int x = info.rng.rndInt(info.pos.x() - ANT_VISION_RADIUS, info.pos.x() + ANT_VISION_RADIUS);
      int y = info.rng.rndInt(info.pos.y() - ANT_VISION_RADIUS, info.pos.y() + ANT_VISION_RADIUS);

      m_target = std::make_shared<utils::Point2i>(x, y);
    }

    AStar astar(info.pos, *m_target, info.locator);
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
    // Determine the type of pheromon based on the mode.
    Scent s;
    switch (m_behavior) {
      case Behavior::Return:
        s = Scent::Food;
        break;
      default:
        // Any other behavior spawns pheromons to come
        // back home.
        s = Scent::Home;
        break;
    }

    // Small randomness in the amount and evaporation rate
    // of each pheromon.
    float a = info.rng.rndFloat(1.0f, 1.1f);
    float e = info.rng.rndFloat(0.1f, 0.12f);

    info.spawned.push_back(Animat{
      info.pos,
      std::make_shared<Pheromon>(s, a, e)
    });

    // Update the variables tracking the spawn of a new
    // pheromone.
    m_lastPheromon = info.moment;
  }

  void
  Ant::wander(Info& info) {
    /// TODO: Handle the wandering behavior.
    if (info.path.empty()) {
      m_target.reset();
      generatePath(info);
    }
  }

  void
  Ant::food(Info& info) {
    /// TODO: Handle the food behavior.
    if (info.path.empty()) {
      m_target.reset();
      generatePath(info);
    }
  }

  void
  Ant::returnHome(Info& info) {
    /// TODO: Handle the return home behavior.
    if (info.path.empty()) {
      m_target.reset();
      generatePath(info);
    }
  }

}
