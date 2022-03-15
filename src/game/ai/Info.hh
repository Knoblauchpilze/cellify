#ifndef    INFO_HH
# define   INFO_HH

# include <vector>
# include <maths_utils/Point2.hh>
# include <core_utils/RNG.hh>
# include "Path.hh"
# include "Locator.hh"
# include "Time.hh"

namespace cellify {

  /// @brief - Forward declaration of the AI class.
  class AI;

  /// @brief - Forward declaration of the AI pointer.
  using AIShPtr = std::shared_ptr<AI>;

  /// @brief - A list of AIs.
  using AIs = std::vector<AIShPtr>;

  /// @brief - Convenience structure defining the info to
  /// use to create a new agent.
  struct Animat {
    // The position of the agent at the moment of its
    // creation.
    utils::Point2i pos;

    // The brain associated to the animat. May be null in
    // case the animat is a simple element.
    AIShPtr brain;
  };

  /// @brief - A list of spawned elements.
  using Animats = std::vector<Animat>;

  /// @brief - Forward declaration of the influence class.
  class Influence;

  /// @brief - Forward declaratrion of the influence pointer.
  using InfluenceShPtr = std::shared_ptr<Influence>;

  /// @brief - A list of influences.
  using Influences = std::vector<InfluenceShPtr>;

  /// @brief - The information needed by the AI to perform
  /// its processing.
  struct Info {
    // The position of the agent.
    utils::Point2i pos;

    // A random number generator to use if needed for random
    // processes during the step.
    utils::RNG& rng;

    // The moment at which the processing is taking place.
    TimeStamp moment;

    // The time elapsed since the last call to this method.
    float elapsed;

    // The current path followed by the element.
    Path& path;

    // The locator service allowing to determine whether
    // cells are obstructed or not.
    Locator& locator;

    // Whether or not this element needs to be marked for
    // deletion.
    bool selfDestruct;

    // A list of new AIs that might be created by this
    // agent.
    Animats spawned;

    // A list of the influences produced by this agent.
    Influences actions;
  };

}

#endif    /* INFO_HH */
