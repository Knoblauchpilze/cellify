
# include "Ant.hh"
# include "AStar.hh"

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
      case Behavior::Deposit:
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
      std::make_shared<Pheromon>(s, info.moment, a, e)
    });

    // Update the variables tracking the spawn of a new
    // pheromone.
    m_lastPheromon = info.moment;
  }

  void
  Ant::wander(Info& info, const std::vector<int>& items) {
    // Check for for food sources and find the closest one.
    utils::Point2i best;
    if (findClosest(info, items, Tile::Food, best)) {
      // In case the path is not yet directed towards
      // this colony, generate a new path.
      if (info.path.end() == best) {
        return;
      }

      log("Found food source at " + best.toString());

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
    if (!aggregatePheromomns(info, items, Scent::Food, avg)) {
      // No pheromons of this type were found, use a
      // random target.
      if (info.path.empty()) {

        log("No relevant food pheromon(s) seen by ant (out of " + std::to_string(items.size()) + " element(s)), choosing random position");

        m_target.reset();
        generatePath(info);
      }

      return;
    }

    // In case the average is the same as the target (which
    // means we didn't find a new pheromon) continue on the
    // same path.
    if (avg == *m_target) {
      return;
    }

    log("Picked target " + avg.toString() + " from " + std::to_string(items.size()) + " visible item(s)");

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
    // Check for for colonies and find the closest one.
    utils::Point2i best;
    if (findClosest(info, items, Tile::Colony, best)) {
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
    // for food.
    utils::Point2i avg;
    if (!aggregatePheromomns(info, items, Scent::Home, avg)) {
      // No pheromons of this type were found, use a
      // random target.
      if (info.path.empty()) {

        log("No relevant home pheromon(s) seen by ant (out of " + std::to_string(items.size()) + " element(s)), choosing random position");

        m_target.reset();
        generatePath(info);
      }

      return;
    }

    // In case the average is the same as the target (which
    // means we didn't find a new pheromon) continue on the
    // same path.
    if (avg == *m_target) {
      return;
    }

    log("Picked target " + avg.toString() + " from " + std::to_string(items.size()) + " visible item(s)");

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

  bool
  Ant::findClosest(Info& info,
                   const std::vector<int>& items,
                   const Tile& tile,
                   utils::Point2i& out) const noexcept
  {
    out = utils::Point2i();
    float d = std::numeric_limits<float>::max();
    bool found = false;

    // Check for for elements of the input type and find
    // the closest one.
    for (unsigned id = 0u ; id < items.size() ; ++id) {
      const Element* el = reinterpret_cast<const Element*>(info.locator.get(items[id]));

      if (el->type() != tile) {
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
      out = el->pos();
    }

    return found;
  }

  bool
  Ant::aggregatePheromomns(Info& info,
                           const std::vector<int>& items,
                           const Scent& scent,
                           utils::Point2i& out) const noexcept
  {
    unsigned count = 0u;
    float weight = 0.0f;

    utils::Point2f temp;
    out = utils::Point2i(0, 0);

    // While aggregating the pheromons, we want to give a
    // higher priority to the pheromons that are older. It
    // indicates that they managed to 'survive' for longer
    // and so might be more relevant for the ant.
    // We first have to accumulate the total weight that
    // is desired, so that we can somehow normalize all
    // of the individual weights.
    float min = std::numeric_limits<float>::max();

    for (unsigned id = 0u ; id < items.size() ; ++id) {
      const Element* el = reinterpret_cast<const Element*>(info.locator.get(items[id]));
      if (el->type() != Tile::Pheromon) {
        continue;
      }

      // Only consider food which leads to food.
      Scent sc = *reinterpret_cast<const Scent*>(el->data());
      if (sc != scent) {
        continue;
      }

      // The weight is based on the creation time.
      TimeStamp ts = *reinterpret_cast<const TimeStamp*>(el->data() + sizeof(Scent));
      Duration d = info.moment - ts;

      // A crude estimate is to convert to seconds, and then to
      // use that value as a weight: the longer a pheromon has
      // been around, the more important it will be considered.
      float w = d / 1000.0f;
      if (w < min) {
        min = w;
      }

      weight += w;
      ++count;
    }

    // In case we didn't find any target, fallback to
    // random target.
    if (count == 0u) {
      return false;
    }

    for (unsigned id = 0u ; id < items.size() ; ++id) {
      const Element* el = reinterpret_cast<const Element*>(info.locator.get(items[id]));
      if (el->type() != Tile::Pheromon) {
        continue;
      }

      // Only consider food which leads to food.
      Scent sc = *reinterpret_cast<const Scent*>(el->data());
      if (sc != scent) {
        continue;
      }

      // The weight is based on the creation time.
      TimeStamp ts = *reinterpret_cast<const TimeStamp*>(el->data() + sizeof(Scent));
      Duration d = info.moment - ts;

      // A crude estimate is to convert to seconds, and then to
      // use that value as a weight: the longer a pheromon has
      // been around, the more important it will be considered.
      float w = d / 1000.0f - min;

      temp.x() += w * el->pos().x();
      temp.y() += w * el->pos().y();

      weight += w;

      ++count;
    }

    out.x() = static_cast<int>(std::round(temp.x() / weight));
    out.y() = static_cast<int>(std::round(temp.y() / weight));

    return true;
  }

}
