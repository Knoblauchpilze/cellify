
# include "Ant.hh"
# include <cxxabi.h>
# include "AStar.hh"
# include "FoodInteraction.hh"

/// @brief - The vision frustim of an ant: defines
/// how far it can perceive blocks. It is also used
/// to define how far a random target can be picked
/// from the current ant's position.
# define ANT_VISION_RADIUS 5

/// @brief - The interval between emitting a new
/// pheromone. Expressed in milliseconds.
# define PHEROMON_SPAWN_INTERVAL 500

/// @brief - The amount of food that the ant can
/// carry in one go.
# define ANT_CARGO_SPACE 5.0f

/// @brief - The minium evaporation rate for pheromons.
# define PHEROMON_EVAPORATION_RATE 0.15f

namespace {

  std::vector<int>
  filterPheromons(const cellify::Info& info,
                  const std::vector<int>& items,
                  const cellify::Scent& scent) noexcept
  {
    std::vector<int> filtered;

    for (unsigned id = 0u ; id < items.size() ; ++id) {
      const cellify::Element* el = reinterpret_cast<const cellify::Element*>(info.locator.get(items[id]));
      if (el->type() != cellify::Tile::Pheromon) {
        continue;
      }

      // Only consider pheromon with the specified scent.
      cellify::Scent sc = *reinterpret_cast<const cellify::Scent*>(el->data());
      if (sc != scent) {
        continue;
      }

      filtered.push_back(items[id]);
    }

    return filtered;
  }

}

namespace cellify {

  std::string
  behaviorToString(const Behavior& b) noexcept {
    switch (b) {
      case Behavior::Wander:
        return "wander";
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

    m_target(nullptr),
    m_randomTarget(false),
    m_lastPos(),
    m_dir(),

    m_food(0.0f)
  {}

  Behavior
  Ant::mode() const noexcept {
    return m_behavior;
  }

  void
  Ant::init(Info& info) {
    generatePath(info);

    m_lastPheromon = info.moment;
    m_lastPos = info.path.begin();

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

    // Change the current position if it changed.
    if (m_lastPos != info.pos) {
      m_dir = info.pos - m_lastPos;
      m_lastPos = info.pos;
    }
  }

  bool
  Ant::influence(const Influence* inf,
                 const Element* body) noexcept
  {
    // In case the influence is not a food interaction,
    // we can't process it.
    const FoodInteraction* fi = dynamic_cast<const FoodInteraction*>(inf);
    if (fi == nullptr) {
      int status;
      std::string it = abi::__cxa_demangle(typeid(*inf).name(), 0, 0, &status);

      error(
        "Failed to process influence",
        "Unsupported influence with kind " + it
      );
    }

    float a = fi->amount(body);
    if (a > 0.0f) {
      debug("Gathered " + std::to_string(a) + " food");
    }
    else {
      debug("Deposit " + std::to_string(-a) + " food");
    }

    m_food += a;

    return true;
  }

  void
  Ant::log(const std::string& message) const noexcept
  {
    // Helps with debugging by prepending the behavior to any log.
    verbose("[" + behaviorToString(m_behavior) + "] " + message);
  }

  bool
  Ant::generatePath(Info& info) {
    // Pick a random target and find a path to it if needed.
    m_randomTarget = false;
    if (m_target == nullptr) {
      int x = info.rng.rndInt(info.pos.x() - ANT_VISION_RADIUS, info.pos.x() + ANT_VISION_RADIUS);
      int y = info.rng.rndInt(info.pos.y() - ANT_VISION_RADIUS, info.pos.y() + ANT_VISION_RADIUS);

      m_target = std::make_shared<utils::Point2i>(x, y);
      m_randomTarget = true;
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
    float e = info.rng.rndFloat(PHEROMON_EVAPORATION_RATE, PHEROMON_EVAPORATION_RATE + 0.1f * PHEROMON_EVAPORATION_RATE);

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
    followPheromonToTarget(info, items, Scent::Food, Tile::Food, Behavior::Food);
  }

  void
  Ant::food(Info& info, const std::vector<int>& items) {
    // We don't have to do anything as long as we didn't
    // reach the food. Then we have to go back home.
    if (!info.path.empty()) {
      return;
    }

    // Fetch the food deposit that we reached, and our
    // own body. We consider that if an element has the
    // same kind and position as the target, it is the
    // target.
    Element* deposit = nullptr;
    Element* body = nullptr;

    unsigned id = 0u;
    unsigned found = 0u;
    while (id < items.size() && found < 2u) {
      const Element* el = reinterpret_cast<const Element*>(info.locator.get(items[id]));

      // Handle the deposit.
      if (el->type() == Tile::Food && el->pos() == *m_target) {
        deposit = const_cast<Element*>(el);
        ++found;
      }

      // Handle the body: we add a test on the identifier
      // as we have it.
      if (el->type() == Tile::Ant && el->pos() == *m_target && el->uuid() == uuid()) {
        body = const_cast<Element*>(el);
        ++found;
      }

      ++id;
    }

    // Can happen that the deposit is empty if somebody else
    // emptied it in the meantime. In this case we want to
    // go back to wandering.
    if (deposit == nullptr) {
      m_behavior = Behavior::Wander;
      return;
    }

    // Create an influence to pick up some food.
    info.actions.push_back(std::make_shared<FoodInteraction>(
      deposit, ANT_CARGO_SPACE, body
    ));

    log("Reached food at " + info.pos.toString() + ", going back home");

    // And change the behavior.
    m_behavior = Behavior::Return;
  }

  void
  Ant::returnHome(Info& info, const std::vector<int>& items) {
    followPheromonToTarget(info, items, Scent::Home, Tile::Colony, Behavior::Deposit);
  }

  void
  Ant::deposit(Info& info, const std::vector<int>& items) {
    // We don't have to do anything as long as we didn't
    // reach the colony. Then we have to go back home.
    if (!info.path.empty()) {
      return;
    }

    // Fetch the food deposit that we got.
    Element* colony = nullptr;
    Element* body = nullptr;

    unsigned id = 0u;
    unsigned found = 0u;
    while (id < items.size() && found < 2u) {
      const Element* el = reinterpret_cast<const Element*>(info.locator.get(items[id]));

      // Handle the colony.
      if (el->type() == Tile::Colony && el->pos() == *m_target) {
        colony = const_cast<Element*>(el);
        ++found;
      }

      // Handle the body: we add a test on the identifier
      // as we have it.
      if (el->type() == Tile::Ant && el->pos() == *m_target && el->uuid() == uuid()) {
        body = const_cast<Element*>(el);
        ++found;
      }

      ++id;
    }

    // Create an influence to deposit some food.
    info.actions.push_back(std::make_shared<FoodInteraction>(
      body, ANT_CARGO_SPACE, colony
    ));

    log("Reached colony at " + info.pos.toString() + ", going back to wander");

    // And change the behavior.
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
                           utils::Point2i& out,
                           bool& reverse) const noexcept
  {
    out = utils::Point2i(0, 0);

    // Filter to keep only pheromons.
    std::vector<int> ph = filterPheromons(info, items, scent);

    // In case there are none, we couldn't find a target.
    if (ph.empty()) {
      return false;
    }

    // Aggregate the average position of the pheromons. We
    // will only consider pheromons that are pointing in
    // the general direction of the ant.
    utils::Point2f temp;
    unsigned count = 0u;

    for (unsigned id = 0u ; id < ph.size() ; ++id) {
      const Element* el = reinterpret_cast<const Element*>(info.locator.get(ph[id]));

      // Discard pheromons that are not in the general way
      // the ant is moving. Also, discard pheromons exactly
      // at our current location.
      utils::Vector2i toPheromon = el->pos() - info.pos;
      float dot = toPheromon * m_dir;
      if (dot < 0.0f) {
        continue;
      }

      if (el->pos() == info.pos) {
        continue;
      }

      temp.x() += el->pos().x();
      temp.y() += el->pos().y();

      ++count;
    }

    // In case all pheromons were discarding, allow the ant
    // to change direction.
    if (count == 0u) {
      reverse = true;
      return false;
    }

    out.x() = static_cast<int>(1.0f * std::round(temp.x() / count));
    out.y() = static_cast<int>(1.0f * std::round(temp.y() / count));

    // Check that the picked target is not obstructed: if
    // yes, then we have to nudge it a bit so that it is
    // not.
    if (info.locator.obstructed(out)) {
      utils::Point2i old = out;
      // We want to spiral outwards of the point and find
      // the first point that is not obstructed. The code
      // to loop in a spiral was taken from here:
      // https://stackoverflow.com/questions/398299/looping-in-a-spiral
      int sx = 0;
      int sy = 0;
      int d = 1;
      int m = 1;

      bool ob = true;

      while (ob) {
        // Move along the `x` axis.
        while (2 * sx * d < m && ob) {
          utils::Point2i cur(out.x() + sx, out.y() + sy);
          ob = info.locator.obstructed(cur);

          if (ob) {
            sx += d;
          }
        }

        // Move along the `y` axis.
        while (2 * sy * d < m && ob) {
          utils::Point2i cur(out.x() + sx, out.y() + sy);
          ob = info.locator.obstructed(cur);

          if (ob) {
            sy += d;
          }
        }

        d = -d;
        ++m;
      }

      out.x() += sx;
      out.y() += sy;

      log("Moved obstructed target " + old.toString() + " to " + out.toString());
    }

    return true;
  }

  void
  Ant::followPheromonToTarget(Info& info,
                              const std::vector<int>& items,
                              const Scent& scent,
                              const Tile& tile,
                              const Behavior& next)
  {
    // Check for for colonies and find the closest one.
    utils::Point2i best;
    if (findClosest(info, items, tile, best)) {
      // In case the path is not yet directed towards
      // this colony, generate a new path.
      if (!info.path.empty() && info.path.end() == best) {
        return;
      }

      log("Found " + tileToString(tile) + " at " + best.toString());

      m_target = std::make_shared<utils::Point2i>(best.x(), best.y());
      generatePath(info);

      // Update the behavior.
      m_behavior = next;

      return;
    }

    // In case we have a path and the target was not
    // picked at random, we can continue on this path
    // until the end.
    if (!m_randomTarget && !info.path.empty()) {
      return;
    }

    // In case we didn't find anything, pick a target
    // which the average of the position of pheromons
    // for food.
    utils::Point2i avg;
    bool reverse = false;
    if (!aggregatePheromomns(info, items, scent, avg, reverse)) {
      // In case we have a valid path, continue on it.
      if (!info.path.empty()) {
        return;
      }

      // Otherwise, determine whether we should reverse
      // the direction of the ant.
      if (reverse) {
        log("Relevant " + scentToString(scent) + " pheromon(s) are behind the ant (out of " + std::to_string(items.size()) + " element(s)), choosing random position");
        m_dir = -m_dir;
        return;
      }

      // No pheromons, pick a random location.
      log("No relevant " + scentToString(scent) + " pheromon(s) seen by ant (out of " + std::to_string(items.size()) + " element(s)), choosing random position");

      m_target.reset();
      generatePath(info);

      return;
    }

    // In case the average is the same as the target (which
    // means we didn't find a new pheromon) continue on the
    // same path.
    if (m_target != nullptr && avg == *m_target) {
      return;
    }

    log("Picked target " + avg.toString() + " to return to from " + std::to_string(items.size()) + " visible item(s)");

    m_target = std::make_shared<utils::Point2i>(avg.x(), avg.y());
    generatePath(info);
  }

}
