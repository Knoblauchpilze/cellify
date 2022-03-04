#ifndef    INFO_HH
# define   INFO_HH

# include <maths_utils/Point2.hh>
# include <core_utils/RNG.hh>
# include <core_utils/TimeUtils.hh>
# include "Locator.hh"

namespace cellify {

  /// @brief - The information needed by the AI to perform
  /// its processing.
  struct Info {
    // The position of the agent.
    utils::Point2i pos;

    // A random number generator to use if needed for random
    // processes during the step.
    utils::RNG& rng;

    // The moment at which the processing is taking place.
    utils::TimeStamp moment;

    // The time elapsed since the last call to this method.
    float elapsed;

    // The locator service allowing to determine whether
    // cells are obstructed or not.
    Locator& locator;

    // Whether or not this element needs to be marked for
    // deletion.
    bool selfDestruct;
  };

}

#endif    /* INFO_HH */
