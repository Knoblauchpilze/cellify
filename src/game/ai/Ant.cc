
# include "Ant.hh"
# include "AStar.hh"
# include "Pheromon.hh"
# include "Element.hh"

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
      case Behavior::Deposit:
        return "deposit";
      default:
        return "unknown";
    }
  }

  Ant::Ant(const utils::Uuid& uuid):
    AI("ant-" + uuid.toString()),

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
    std::vector<int> items = info.locator.visible(info.pos, ANT_VISION_RADIUS);;

    switch (m_behavior) {
      case Behavior::Food:
        food(info, items);
        break;
      case Behavior::Return:
        returnHome(info, items);
        break;
      case Behavior::Deposit:
        deposit(info, items);
        break;
      default:
        warn("Unknown behavior " + behaviorToString(m_behavior));
        [[fallthrough]];
      case Behavior::Wander:
        wander(info, items);
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
  Ant::wander(Info& info, const std::vector<int>& items) {
    // Check for foor sources and find the closest one.
    utils::Point2i best = info.pos;
    float d = std::numeric_limits<float>::max();
    bool found = false;

    for (unsigned id = 0u ; id < items.size() ; ++id) {
      const Element* el = reinterpret_cast<const Element*>(info.locator.get(items[id]));

      if (el->type() != Tile::Food) {
        continue;
      }

      found = true;
      float dx = el->pos().x() - info.pos.x();
      float dy = el->pos().y() - info.pos.y();
      float dist = std::sqrt(dx * dx + dy * dy);

      if (dist > d) {
        continue;
      }

      d = dist;
      best = el->pos();
    }

    if (found) {
      // In case the path is not yet directed towards
      // this food source, generate a new path.
      if (info.path.end() == best) {
        return;
      }

      log("Found food at " + best.toString());

      m_target = std::make_shared<utils::Point2i>(best.x(), best.y());
      generatePath(info);

      // Update the behavior.
      m_behavior = Behavior::Food;

      return;
    }

    // In case we didn't find anything, pick a target
    // which the average of the position of pheromons
    // for food.
    utils::Point2i avg;
    unsigned count = 0u;

    for (unsigned id = 0u ; id < items.size() ; ++id) {
      const Element* el = reinterpret_cast<const Element*>(info.locator.get(items[id]));
      if (el->type() != Tile::Pheromon) {
        continue;
      }

      // Only consider food which leads to food.
      Scent scent = *reinterpret_cast<const Scent*>(el->data());
      if (scent != Scent::Food) {
        continue;
      }

      avg.x() += el->pos().x();
      avg.y() += el->pos().y();
      ++count;
    }

    // In case we didn't find any target, fallback to
    // random target.
    if (count == 0u) {
      if (info.path.empty()) {

        log("Nothing relevant seen by ant (out of " + std::to_string(items.size()) + " element(s)), choosing random position");

        m_target.reset();
        generatePath(info);
      }

      return;
    }

    avg.x() = static_cast<int>(std::round(1.0f * avg.x() / count));
    avg.y() = static_cast<int>(std::round(1.0f * avg.y() / count));

    // In case the average is the same as the target (which
    // means we didn't find a new pheromon) continue on the
    // same path.
    if (avg == *m_target) {
      return;
    }

    log("Picked target " + avg.toString() + " from " + std::to_string(count) + " food pheromon(s)");

    m_target = std::make_shared<utils::Point2i>(avg.x(), avg.y());
    generatePath(info);
  }

  void
  Ant::food(Info& info, const std::vector<int>& /*items*/) {
    // We don't have to do anything as long as we didn't
    // reach the food. Then we have to go back home.
    if (!info.path.empty()) {
      return;
    }

    log("Reached food at " + info.pos.toString() + ", going back home");
    m_behavior = Behavior::Return;
  }

  void
  Ant::returnHome(Info& info, const std::vector<int>& items) {
    // Check for foor colonies and find the closest one.
    utils::Point2i best = info.pos;
    float d = std::numeric_limits<float>::max();
    bool found = false;

    for (unsigned id = 0u ; id < items.size() ; ++id) {
      const Element* el = reinterpret_cast<const Element*>(info.locator.get(items[id]));

      if (el->type() != Tile::Colony) {
        continue;
      }

      found = true;
      float dx = el->pos().x() - info.pos.x();
      float dy = el->pos().y() - info.pos.y();
      float dist = std::sqrt(dx * dx + dy * dy);

      if (dist > d) {
        continue;
      }

      d = dist;
      best = el->pos();
    }

    if (found) {
      // In case the path is not yet directed towards
      // this colony, generate a new path.
      if (info.path.end() == best) {
        return;
      }

      log("Found colony at " + best.toString());

      m_target = std::make_shared<utils::Point2i>(best.x(), best.y());
      generatePath(info);

      // Update the behavior.
      m_behavior = Behavior::Deposit;

      return;
    }

    // In case we didn't find anything, pick a target
    // which the average of the position of pheromons
    // for home.
    utils::Point2i avg;
    unsigned count = 0u;

    for (unsigned id = 0u ; id < items.size() ; ++id) {
      const Element* el = reinterpret_cast<const Element*>(info.locator.get(items[id]));
      if (el->type() != Tile::Pheromon) {
        continue;
      }

      // Only consider food which leads to food.
      Scent scent = *reinterpret_cast<const Scent*>(el->data());
      if (scent != Scent::Home) {
        continue;
      }

      avg.x() += el->pos().x();
      avg.y() += el->pos().y();
      ++count;
    }


    // In case we didn't find any target, fallback to
    // random target.
    if (count == 0u) {
      if (info.path.empty()) {

        log("Nothing relevant seen by ant (out of " + std::to_string(items.size()) + " element(s)), choosing random position");

        m_target.reset();
        generatePath(info);
      }

      return;
    }

    avg.x() = static_cast<int>(std::round(1.0f * avg.x() / count));
    avg.y() = static_cast<int>(std::round(1.0f * avg.y() / count));

    // In case the average is the same as the target (which
    // means we didn't find a new pheromon) continue on the
    // same path.
    if (avg == *m_target) {
      return;
    }

    log("Picked target " + avg.toString() + " from " + std::to_string(count) + " home pheromon(s)");

    m_target = std::make_shared<utils::Point2i>(avg.x(), avg.y());
    generatePath(info);
  }

  void
  Ant::deposit(Info& info, const std::vector<int>& /*items*/) {
    // We don't have to do anything as long as we didn't
    // reach the colony. Then we have to go back home.
    if (!info.path.empty()) {
      return;
    }

    log("Reached colony at " + info.pos.toString() + ", going back to wander");
    m_behavior = Behavior::Wander;
  }

}
