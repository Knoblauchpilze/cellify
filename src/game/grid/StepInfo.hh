#ifndef    STEP_INFO_HH
# define   STEP_INFO_HH

# include <vector>
# include <core_utils/RNG.hh>
# include <core_utils/TimeUtils.hh>
# include "Element.hh"

namespace cellify {

  /// @brief - Convenience structure regrouping all variables
  /// needed to perform the advancement of one step of a world
  /// object. It includes a RNG, info on the dimensions of the
  /// world, etc.
  struct StepInfo {
    // A random number generator to use if needed for random
    // processes during the step.
    utils::RNG& rng;

    // The moment at which the processing is taking place.
    utils::TimeStamp moment;

    // The time elapsed since the last call to this method.
    float elapsed;

    // The list of elements that will be spawned after the
    // end of the step.
    std::vector<ElementShPtr> spawned;
  };

}

#endif    /* STEP_INFO_HH */
