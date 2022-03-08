#ifndef    ANT_HH
# define   ANT_HH

# include "AI.hh"
# include "Time.hh"

namespace cellify {

  class Ant: public AI {
    public:

      /**
       * @brief - Creates a new ant.
       */
      Ant();

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
       * @brief - The last time a pheromon was emitted by this
       *          entity.
       */
      TimeStamp m_lastPheromon;
  };

}

#endif    /* ANT_HH */
