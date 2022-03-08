#ifndef    TIME_HH
# define   TIME_HH

namespace cellify {

  /// @brief - A convenience define for a timestamp in the
  /// context of the simulation. We don't want to rely on
  /// the timestamp as defined in the core module as we do
  /// allow simulation to run faster/slower. So we just use
  /// some sort of floating point value to represent the
  /// simulation time.
  using TimeStamp = float;

  /// @brief - To go along the timestamp, we need a way to
  /// measure the duration between two timestamps. This is
  /// allowed by this convenience define. The duration is
  /// expressed in *milliseconds*.
  using Duration = float;

  /**
   * @brief - Generate a zero timestamp.
   * @return - a zero duration.
   */
  TimeStamp
  zero() noexcept;

  /**
   * @brief - Defines a conversion method for a duration
   *          expressed in milliseconds.
   * @param ms - the duration to convert.
   * @return - the corresponding duration.
   */
  Duration
  millisecondsToDuration(float ms) noexcept;

}

#endif    /* TIME_HH */
