#ifndef    ANT_HH
# define   ANT_HH

# include <memory>
# include "AI.hh"
# include "Time.hh"

namespace cellify {

  /// @brief - The potential mode for the ant. Depends on whether
  /// some food was already found or not.
  enum class Behavior {
    Wander,
    Food,
    Return
  };

  class Ant: public AI {
    public:

      /**
       * @brief - Creates a new ant.
       */
      Ant();

      /**
       * @brief - Return the current behavior for the ant.
       * @return - the behavior for this ant.
       */
      Behavior
      mode() const noexcept;

      /**
       * @brief - Implementation of the initialization method.
       * @param info - the info of the step.
       */
      void
      init(Info& info) override;

      /**
       * @brief - Implementation of the step method.
       * @param info - the information to get data on the step.
       */
      void
      step(Info& info) override;

    private:

      /**
       * @brief - Generate a new path to a random location. It
       *          overrides any existing path.
       * @param info - data to generate the path.
       * @return - whether a path was generated.
       */
      bool
      generatePath(Info& info);

      /**
       * @brief - Used to perform the spawning of a pheromon by
       *          this ant, at the current position.
       * @param info - the info to use to spawn the pheromon.
       */
      void
      spawnPheromon(Info& info) noexcept;

    private:

      /**
       * @brief - The beahvior currently active for the ant.
       */
      Behavior m_behavior;

      /**
       * @brief - The last time a pheromon was emitted by this
       *          entity.
       */
      TimeStamp m_lastPheromon;
  };

  using AntShPtr = std::shared_ptr<Ant>;
}

#endif    /* ANT_HH */
