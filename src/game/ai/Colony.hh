#ifndef    COLONY_HH
# define   COLONY_HH

# include <core_utils/Uuid.hh>
# include "AI.hh"
# include "Time.hh"

namespace cellify {

  class Colony: public AI {
    public:

      /**
       * @brief - Creates a new colony.
       * @param uuid - the identifier of the colony.
       */
      Colony(const utils::Uuid& uuid);

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

      /**
       * @brief - Implementation of the interface method to apply
       *          an influence to this colony.
       * @param influence - the influence to process.
       * @param body - the body of the agent.
       * @return - `true` if the influence could be processed.
       */
      bool
      influence(const Influence* inf,
                const Element* body) noexcept override;

    private:

      /**
       * @brief - A convenience method to spawn an agent. Handles
       *          the necessary checks about where to spawn it.
       * @param info - the data to register the spawned agent.
       */
      void
      spawn(Info& info) noexcept;

      /**
       * @brief - Picks a free position in the range provided as
       *          input. If no free position exists we return a
       *          boolean indicating it.
       * @param info - defines the locator to search for a position
       *               along with a random number generator.
       * @param rangeX - the range along the x positions.
       * @param rangeY - the range along the y positions.
       * @param pos - the output position.
       * @return - `true` if a position could be found.
       */
      bool
      pickRandomPosition(const Info& info,
                         const std::pair<int, int>& rangeX,
                         const std::pair<int, int>& rangeY,
                         utils::Point2i& pos) const noexcept;

    private:

      /**
       * @brief - The budget of the colony.
       */
      float m_budget;

      /**
       * @brief - The cost to spawn a single ant.
       */
      float m_antCost;

      /**
       * @brief - The timestamp at which the colony spawned a
       *          new ant last.
       */
      TimeStamp m_lastSpawn;

      /**
       * @brief - The duration between two consecutive spawns
       *          of a new agent.
       */
      Duration m_restTime;
  };

}

#endif    /* COLONY_HH */
