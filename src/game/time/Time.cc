
# include "Time.hh"

namespace cellify {

  TimeStamp
  zero() noexcept {
    return 0.0f;
  }

  Duration
  millisecondsToDuration(float ms) noexcept {
    // No conversion to do.
    return ms;
  }

}
