
# include "Colony.hh"
# include <cxxabi.h>
# include "Ant.hh"
# include "FoodInteraction.hh"

/// @brief - The duration in milliseconds between two
/// consecutive spawn of an agent.
# define ANT_SPAWN_INTERNAL 200

/// @brief - The range around which the agents can be
/// spawned around the colony.
# define ANT_SPAWN_RADIUS 2

namespace cellify {

  Colony::Colony(const utils::Uuid& uuid):
    AI("colony-" + uuid.toString()),

    m_budget(50.0f),
    m_antCost(50.0f),
    m_lastSpawn(zero()),
    m_restTime(millisecondsToDuration(ANT_SPAWN_INTERNAL))
  {}

  void
  Colony::init(Info& info) {
    // Make sure we can spawn an ant right away if needed.
    m_lastSpawn = info.moment - m_restTime;
  }

  void
  Colony::step(Info& info) {
    // If there's enough budget, spawn an ant if it fits
    // with the time constraint.
    if (m_budget >= m_antCost && info.moment > m_lastSpawn + m_restTime) {
      spawn(info);
    }
  }

  bool
  Colony::influence(const Influence* inf,
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
    log("Adding " + std::to_string(a) + " to budget of colony (current: " + std::to_string(m_budget + a) + ")", utils::Level::Info);

    m_budget += a;

    return true;
  }

  void
  Colony::spawn(Info& info) noexcept {
    // Compute ranges.
    std::pair<float, float> rangeX = std::make_pair(
      info.pos.x() - ANT_SPAWN_RADIUS, info.pos.x() + ANT_SPAWN_RADIUS
    );
    std::pair<float, float> rangeY = std::make_pair(
      info.pos.y() - ANT_SPAWN_RADIUS, info.pos.y() + ANT_SPAWN_RADIUS
    );

    // Pick a random position around the colony.
    utils::Point2i p;
    if (!pickRandomPosition(info, rangeX, rangeY, p)) {
      warn(
        "Failed to spawn agent",
        "No empty location around the colony"
      );

      return;
    }

    AIShPtr brain = std::make_shared<Ant>(utils::Uuid::create());

    info.spawned.push_back(Animat{p, brain});

    log(
      "Registering spawn attempt at " + p.toString() +
      " (budget: " + std::to_string(m_budget) + "/" + std::to_string(m_antCost) + ")"
     );

    // Update spawn tracking variables.
    m_lastSpawn = info.moment;
    m_budget -= m_antCost;
  }

  bool
  Colony::pickRandomPosition(const Info& info,
                             const std::pair<int, int>& rangeX,
                             const std::pair<int, int>& rangeY,
                             utils::Point2i& pos) const noexcept
  {
    // Check range validity.
    if (rangeX.second < rangeX.first || rangeY.second < rangeY.first) {
      warn(
        "Failed to pick random position",
        "Invalid ranges " + std::to_string(rangeX.first) + " - " + std::to_string(rangeX.second) +
        " and " + std::to_string(rangeX.first) + " - " + std::to_string(rangeY.second)
      );

      return false;
    }

    // Assume we can find a position in less than the amount
    // of cells that are defined in the range.
    unsigned attempt = 0u;
    unsigned count = (rangeX.second - rangeX.first) * (rangeY.second - rangeY.first);
    bool obstructed = true;

    while (attempt < count && obstructed) {
      pos.x() = info.rng.rndInt(rangeX.first, rangeX.second);
      pos.y() = info.rng.rndInt(rangeY.first, rangeY.second);

      obstructed = info.locator.obstructed(pos);

      ++attempt;
    }

    if (obstructed) {
      return false;
    }

    return true;
  }

}
