#ifndef    STEP_INFO_HH
# define   STEP_INFO_HH

# include <vector>
# include <memory>
# include <core_utils/RNG.hh>
# include "Time.hh"

namespace cellify {

  /// @brief - Forward declaration of the element class.
  class Element;

  /// @brief - Forward declaration of the element pointer.
  using ElementShPtr = std::shared_ptr<Element>;

  /// @brief - A list of elements.
  using Elements = std::vector<ElementShPtr>;

  /// @brief - Forward declaration of a grid.
  class Grid;

  /// @brief - Convenience structure regrouping all variables
  /// needed to perform the advancement of one step of a world
  /// object. It includes a RNG, info on the dimensions of the
  /// world, etc.
  struct StepInfo {
    // A random number generator to use if needed for random
    // processes during the step.
    utils::RNG& rng;

    // The moment at which the processing is taking place.
    TimeStamp moment;

    // The time elapsed since the last call to this method.
    float elapsed;

    // The grid allowing to detect obstruction in cells and
    // generally get info about the world.
    Grid& grid;

    // The list of elements that will be spawned after the
    // end of the step.
    Elements spawned;
  };

}

#endif    /* STEP_INFO_HH */
